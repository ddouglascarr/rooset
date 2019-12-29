FROM debian:buster

RUN apt-get -y update && apt-get install -y \
    build-essential \
    curl \
    jq \
    lua5.3 \
    liblua5.3-dev \
    libbsd-dev \
    libpq-dev \
    lighttpd \
    pmake \
    exim4 \
    postgresql-client \
    postgresql-server-dev-all \
    python-pip \
    sudo \
    wget

RUN pip install markdown2

ADD ./lfmoonbridge /opt/moonbridge
ADD ./lfframework /opt/framework
ADD ./lffrontend /opt/frontend
ADD ./etc/keep-alive /usr/local/bin/keep-alive

# Install Moonbridge
RUN cd /opt/moonbridge && \
  pmake MOONBR_LUA_PATH=/opt/moonbridge/?.lua

ENV MOONBR_LUA_PATH="/opt/moonbridge/?.lua"   
ENV LUA_PATH="/opt/moonbridge/?.lua;./?.lua;./?.lc;"
ENV FRONTEND_CONFIG="docker"

# Install webMCP framework
RUN cd /opt/framework && \
  make

ADD ./etc/frontend-server /usr/local/bin/
Add ./etc /opt/etc

EXPOSE 8080
ENTRYPOINT [ "/usr/local/bin/frontend-server" ]
