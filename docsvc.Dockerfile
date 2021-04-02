FROM golang:1.16-buster

RUN apt-get update && apt-get install -y \
  libxml2 \
  libxml2-dev

# build and install app binary
COPY . /go/src/github.com/ddouglascarr/rooset
WORKDIR /go/src/github.com/ddouglascarr/rooset
RUN go build cmd/rooset.go

