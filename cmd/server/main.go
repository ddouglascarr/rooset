package main

import (
	"github.com/ddouglascarr/rooset/server"
	_ "github.com/lib/pq"
)

func main() {
	err := server.ListenAndServe()
	if err != nil {
		panic(err)
	}
}
