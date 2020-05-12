FROM golang:1.13-buster

RUN apt-get update && apt-get install -y \
  autoconf \
  automake \
  libtool \
  libxml2 \
  libxml2-dev \
  curl \
  make \
  g++ \
  git \
  unzip

# C++ protobuf
RUN cd /opt && git clone https://github.com/protocolbuffers/protobuf.git
WORKDIR /opt/protobuf
RUN git submodule update --init --recursive
RUN ./autogen.sh
RUN ./configure 
RUN make
RUN make check
RUN make install
RUN ldconfig

# golang protobuf
RUN go get -u github.com/golang/protobuf/protoc-gen-go

# js protobuf
RUN apt-get install -y nodejs
RUN apt-get install -y npm
RUN npm install -g protobufjs

# build and install app binary
COPY . /go/src/github.com/ddouglascarr/rooset
WORKDIR /go/src/github.com/ddouglascarr/rooset
RUN go build cmd/rooset.go

