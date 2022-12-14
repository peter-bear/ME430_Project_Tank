package main

import (
	"fmt"
	"github.com/tarm/serial"
	"log"
	"math"
	"net/http"
	"os"
	"runtime"
	"strconv"
)

var g_port *serial.Port

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

func main() {

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
