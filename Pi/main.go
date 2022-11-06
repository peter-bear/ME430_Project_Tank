package main

import (
	"bytes"
	"flag"
	"fmt"
	"github.com/tarm/serial"
	"image"
	"image/jpeg"
	"log"
	"math"
	"mime/multipart"
	"net/http"
	"net/textproto"
	"os"
	"runtime"
	"sort"
	"strconv"
	"time"

	"github.com/blackjack/webcam"
)

var g_port *serial.Port

const (
	V4L2_PIX_FMT_PJPG = 0x47504A50
	V4L2_PIX_FMT_YUYV = 0x56595559
)

type FrameSizes []webcam.FrameSize

func (slice FrameSizes) Len() int {
	return len(slice)
}

// For sorting purposes
func (slice FrameSizes) Less(i, j int) bool {
	ls := slice[i].MaxWidth * slice[i].MaxHeight
	rs := slice[j].MaxWidth * slice[j].MaxHeight
	return ls < rs
}

// For sorting purposes
func (slice FrameSizes) Swap(i, j int) {
	slice[i], slice[j] = slice[j], slice[i]
}

var supportedFormats = map[webcam.PixelFormat]bool{
	V4L2_PIX_FMT_PJPG: true,
	V4L2_PIX_FMT_YUYV: true,
}

func hello(w http.ResponseWriter, req *http.Request) {
	fmt.Println("AAAAAAAAAAAA")
}

func headers(w http.ResponseWriter, req *http.Request) {
	for name, headers := range req.Header {
		for _, h := range headers {
			fmt.Fprintf(w, "%v: %v\n", name, h)
		}
	}
}

func index_handler(w http.ResponseWriter, req *http.Request) {
	content, err := os.ReadFile("index.html")
	if err != nil {
		log.Fatal(err)
	}

	fmt.Fprintf(w, string(content))
}

func joystick_handler(w http.ResponseWriter, req *http.Request) {

	switch req.Method {
	case "GET":
		fmt.Println("Received Get")

	case "POST":
		if err := req.ParseForm(); err != nil {
			fmt.Fprintf(w, "ParseForm() err: %v", err)
			return
		}

		lx, _ := strconv.Atoi(req.FormValue("lx"))
		ly, _ := strconv.Atoi(req.FormValue("ly"))

		var leftstick string = fmt.Sprintf("M %d %d %d\n", lx, ly, int(math.Sqrt(float64((lx-50)*(lx-50)+(ly-50)*(ly-50)))))

		fmt.Printf("Send: %s", leftstick)
		_, err := g_port.Write([]byte(leftstick))
		if err != nil {
			log.Println(err)
		}

		rx, _ := strconv.Atoi(req.FormValue("rx"))
		ry, _ := strconv.Atoi(req.FormValue("ry"))

		var rightstick string = fmt.Sprintf("G %d %d\n", rx, ry)

		fmt.Printf("Send: %s", rightstick)
		_, err = g_port.Write([]byte(rightstick))
		if err != nil {
			log.Println(err)
		}

	default:
		fmt.Println("Other types")
	}
}

func horn_handler(w http.ResponseWriter, req *http.Request) {

	switch req.Method {
	case "GET":
		fmt.Println("Get Horn")
		_, err := g_port.Write([]byte("H\n"))
		if err != nil {
			log.Println(err)
		}

	default:
		fmt.Println("Other types")
	}
}

func fire_handler(w http.ResponseWriter, req *http.Request) {

	switch req.Method {
	case "GET":
		fmt.Println("Get Fire")
		_, err := g_port.Write([]byte("F\n"))
		if err != nil {
			log.Println(err)
		}

	default:
		fmt.Println("Other types")
	}
}

func initCamera() {
	dev := flag.String("d", "/dev/video0", "video device to use")
	fmtstr := flag.String("f", "", "video format to use, default first supported")
	szstr := flag.String("s", "640x480", "frame size to use, default largest one")
	single := flag.Bool("m", false, "single image http mode, default mjpeg video")
	addr := flag.String("l", ":8090", "addr to listien")
	fps := flag.Bool("p", false, "print fps info")
	flag.Parse()

	cam, err := webcam.Open(*dev)
	if err != nil {
		panic(err.Error())
	}
	defer cam.Close()

	// select pixel format
	format_desc := cam.GetSupportedFormats()

	fmt.Println("Available formats:")
	for _, s := range format_desc {
		fmt.Fprintln(os.Stderr, s)
	}

	var format webcam.PixelFormat
FMT:
	for f, s := range format_desc {
		if *fmtstr == "" {
			if supportedFormats[f] {
				format = f
				break FMT
			}

		} else if *fmtstr == s {
			if !supportedFormats[f] {
				log.Println(format_desc[f], "format is not supported, exiting")
				return
			}
			format = f
			break
		}
	}
	if format == 0 {
		log.Println("No format found, exiting")
		return
	}

	// select frame size
	frames := FrameSizes(cam.GetSupportedFrameSizes(format))
	sort.Sort(frames)

	fmt.Fprintln(os.Stderr, "Supported frame sizes for format", format_desc[format])
	for _, f := range frames {
		fmt.Fprintln(os.Stderr, f.GetString())
	}
	var size *webcam.FrameSize
	if *szstr == "" {
		size = &frames[len(frames)-1]
	} else {
		for _, f := range frames {
			if *szstr == f.GetString() {
				size = &f
			}
		}
	}
	if size == nil {
		log.Println("No matching frame size, exiting")
		return
	}

	fmt.Fprintln(os.Stderr, "Requesting", format_desc[format], size.GetString())
	f, w, h, err := cam.SetImageFormat(format, uint32(size.MaxWidth), uint32(size.MaxHeight))
	if err != nil {
		log.Println("SetImageFormat return error", err)
		return

	}
	fmt.Fprintf(os.Stderr, "Resulting image format: %s %dx%d\n", format_desc[f], w, h)

	// start streaming
	err = cam.StartStreaming()
	if err != nil {
		log.Println(err)
		return
	}

	var (
		li   chan *bytes.Buffer = make(chan *bytes.Buffer)
		fi   chan []byte        = make(chan []byte)
		back chan struct{}      = make(chan struct{})
	)
	go encodeToImage(cam, back, fi, li, w, h, f)
	if *single {
		go httpImage(*addr, li)
	} else {
		go httpVideo(*addr, li)
	}

	timeout := uint32(5) //5 seconds
	start := time.Now()
	var fr time.Duration

	for {
		err = cam.WaitForFrame(timeout)
		if err != nil {
			log.Println(err)
			return
		}

		switch err.(type) {
		case nil:
		case *webcam.Timeout:
			log.Println(err)
			continue
		default:
			log.Println(err)
			return
		}

		frame, err := cam.ReadFrame()
		if err != nil {
			log.Println(err)
			return
		}
		if len(frame) != 0 {

			// print framerate info every 10 seconds
			fr++
			if *fps {
				if d := time.Since(start); d > time.Second*10 {
					fmt.Println(float64(fr)/(float64(d)/float64(time.Second)), "fps")
					start = time.Now()
					fr = 0
				}
			}

			select {
			case fi <- frame:
				<-back
			default:
			}
		}
	}
}

func encodeToImage(wc *webcam.Webcam, back chan struct{}, fi chan []byte, li chan *bytes.Buffer, w, h uint32, format webcam.PixelFormat) {

	var (
		frame []byte
		img   image.Image
	)
	for {
		bframe := <-fi
		// copy frame
		if len(frame) < len(bframe) {
			frame = make([]byte, len(bframe))
		}
		copy(frame, bframe)
		back <- struct{}{}

		switch format {
		case V4L2_PIX_FMT_YUYV:
			yuyv := image.NewYCbCr(image.Rect(0, 0, int(w), int(h)), image.YCbCrSubsampleRatio422)
			for i := range yuyv.Cb {
				ii := i * 4
				yuyv.Y[i*2] = frame[ii]
				yuyv.Y[i*2+1] = frame[ii+2]
				yuyv.Cb[i] = frame[ii+1]
				yuyv.Cr[i] = frame[ii+3]

			}
			img = yuyv
		default:
			log.Fatal("invalid format ?")
		}
		//convert to jpeg
		buf := &bytes.Buffer{}
		if err := jpeg.Encode(buf, img, nil); err != nil {
			log.Fatal(err)
			return
		}

		const N = 50
		// broadcast image up to N ready clients
		nn := 0
	FOR:
		for ; nn < N; nn++ {
			select {
			case li <- buf:
			default:
				break FOR
			}
		}
		if nn == 0 {
			li <- buf
		}

	}
}

func httpImage(addr string, li chan *bytes.Buffer) {
	http.HandleFunc("/api/stream/", func(w http.ResponseWriter, r *http.Request) {
		log.Println("connect from", r.RemoteAddr, r.URL)

		//remove stale image
		<-li

		img := <-li

		w.Header().Set("Content-Type", "image/jpeg")

		if _, err := w.Write(img.Bytes()); err != nil {
			log.Println(err)
			return
		}

	})

	log.Fatal(http.ListenAndServe(addr, nil))
}

func httpVideo(addr string, li chan *bytes.Buffer) {
	http.HandleFunc("/api/stream/", func(w http.ResponseWriter, r *http.Request) {
		log.Println("connect from", r.RemoteAddr, r.URL)

		//remove stale image
		<-li
		const boundary = `frame`
		w.Header().Set("Content-Type", `multipart/x-mixed-replace;boundary=`+boundary)
		multipartWriter := multipart.NewWriter(w)
		multipartWriter.SetBoundary(boundary)
		for {
			img := <-li
			image := img.Bytes()
			iw, err := multipartWriter.CreatePart(textproto.MIMEHeader{
				"Content-type":   []string{"image/jpeg"},
				"Content-length": []string{strconv.Itoa(len(image))},
			})
			if err != nil {
				log.Println(err)
				return
			}
			_, err = iw.Write(image)
			if err != nil {
				log.Println(err)
				return
			}
		}
	})

	log.Fatal(http.ListenAndServe(addr, nil))
}

func main() {

	go initCamera()

	var ser *serial.Config
	//设置串口编号
	if runtime.GOOS == "windows" {
		fmt.Println("Running on Windows")
		ser = &serial.Config{Name: "COM5", Baud: 115200}
	} else {
		fmt.Println("Running on " + runtime.GOOS)
		ser = &serial.Config{Name: "/dev/ttyACM0", Baud: 115200}
	}

	//打开串口
	g_port, _ = serial.OpenPort(ser)

	http.HandleFunc("/", index_handler)
	http.HandleFunc("/hello/", hello)
	http.HandleFunc("/headers", headers)

	http.HandleFunc("/api/joystick/", joystick_handler)
	http.HandleFunc("/api/horn/", horn_handler)
	http.HandleFunc("/api/fire/", fire_handler)

	go http.ListenAndServe(":8090", nil)

	for {

		var input string
		var serialComplete bool = false

		for !serialComplete {

			buf := make([]byte, 128)
			lens, _ := g_port.Read(buf)

			if buf[lens-1] != '\n' {
				input = input + string(buf[:lens])
			} else {
				serialComplete = true
			}

		}

		fmt.Printf("Receive: %s\n", input)

	}

}
