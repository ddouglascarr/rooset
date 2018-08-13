package main

import (
	"fmt"
	"log"
	"net/http"
)

func units(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hello world")
}

func main() {
	if err := run(); err != nil {
		log.Fatal(err)
	}
}

func run() error {
	var h = http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "Hello world")
	})

	mux := http.NewServeMux()
	mux.Handle("/units", h)

	srv := http.Server{
		Addr:    ":12345",
		Handler: mux,
	}

	if err := srv.ListenAndServe(); err != nil {
		return err
	}
	return nil
}
