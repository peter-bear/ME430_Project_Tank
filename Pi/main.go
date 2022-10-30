package main

import (
	"fmt"
	"github.com/tarm/serial"
	"log"
	"net/http"
	"os"
	"time"
)

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
		fmt.Println(req.PostForm)

	default:
		fmt.Println("Other types")
	}
}

func main() {

	http.HandleFunc("/", index_handler)
	http.HandleFunc("/hello/", hello)
	http.HandleFunc("/headers", headers)
	
	http.HandleFunc("/api/joystick/", joystick_handler)
	
	http.ListenAndServe(":8090", nil)


}
