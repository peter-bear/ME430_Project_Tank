package main

import (
	"fmt"
	"github.com/tarm/serial"
	"log"
	"math"
	"net/http"
	"os"
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

		//sendData := make([]byte, 5)
		lx, _ := strconv.Atoi(req.FormValue("lx"))
		ly, _ := strconv.Atoi(req.FormValue("ly"))
		//sendData[0] = 1
		//sendData[1] = byte(lx)
		//sendData[2] = byte(ly)
		//sendData[3] = byte(math.Sqrt(float64(lx*lx + ly*ly)))
		//sendData[4] = '\n'

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

		//sendData = make([]byte, 4)
		//rx, _ := strconv.Atoi(req.FormValue("rx"))
		//ry, _ := strconv.Atoi(req.FormValue("ry"))
		//sendData[0] = 2
		//sendData[1] = byte(rx)
		//sendData[2] = byte(ry)
		//sendData[3] = '\n'
		//fmt.Print("Send: ")
		//fmt.Println(sendData)
		//
		//_, err = g_port.Write(sendData)
		//if err != nil {
		//	log.Println(err)
		//}

	default:
		fmt.Println("Other types")
	}
}

func main() {

	//设置串口编号
	ser := &serial.Config{Name: "COM5", Baud: 9600}
	//打开串口
	g_port, _ = serial.OpenPort(ser)

	http.HandleFunc("/", index_handler)
	http.HandleFunc("/hello/", hello)
	http.HandleFunc("/headers", headers)

	http.HandleFunc("/api/joystick/", joystick_handler)

	go http.ListenAndServe(":8090", nil)

	////启动一个协程循环发送
	//go func() {
	//	for {
	//
	//		sendData := make([]byte, 4)
	//		sendData[0] = 5
	//		sendData[1] = 6
	//		sendData[2] = 7
	//		sendData[3] = 255
	//		_, err := g_port.Write(sendData)
	//		if err != nil {
	//			log.Println(err)
	//			continue
	//		}
	//		log.Print("Send: ")
	//		log.Println(sendData)
	//		time.Sleep(time.Second)
	//	}
	//}()

	for {

		var input string
		var serialComplete bool = false

		for !serialComplete {

			buf := make([]byte, 1024)
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
