package main

import (
	"io"
	"net/http"
	"os"
)

func GetEnv(key, defaultValue string) string {
	if value, ok := os.LookupEnv(key); ok {
		return value
	}
	return defaultValue
}

var env = GetEnv("POD_VERSION", "[V0]")

func hello(w http.ResponseWriter, r *http.Request) {
	io.WriteString(w, env+": hello, kubenetes")
}

func main() {
	http.HandleFunc("/", hello)
	http.ListenAndServe(":6763", nil)
}
