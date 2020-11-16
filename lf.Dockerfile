FROM debian:buster

RUN apt-get -y update && apt-get install -y \
    bmake \
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

ADD ./lfmoonbridge /opt/rooset/lfmoonbridge
ADD ./lfframework /opt/rooset/lfframework
ADD ./lffrontend /opt/rooset/lffrontend
ADD ./etc /opt/rooset/etc
ADD ./lfcore /opt/rooset/lfcore

ENV PGHOST="db"
ENV PGUSER="postgres"
ENV PGDATABASE="liquid_feedback"

# lfcore setup

# seconds before first execution
ENV CORE_SERVICES_INITIAL_DELAY="10"  
# seconds between runs
ENV CORE_SERVICES_REPEAT_DELAY="5"

WORKDIR /opt/lfcore
RUN bmake

# lffrontend setup
ADD ./etc/keep-alive /usr/local/bin/keep-alive

# Install Moonbridge
RUN cd /opt/rooset/lfmoonbridge && \
  pmake MOONBR_LUA_PATH=/opt/rooset/lfmoonbridge/?.lua

ENV MOONBR_LUA_PATH="/opt/rooset/lfmoonbridge/?.lua"   
ENV LUA_PATH="/opt/rooset/lffrontend/?.lua;/opt/rooset/lfmoonbridge/?.lua;./?.lua;./?.lc;"
ENV FRONTEND_CONFIG="docker"

# Install webMCP framework
RUN cd /opt/rooset/lfframework && \
  make


EXPOSE 8080
WORKDIR /opt/rooset/etc

# override with /opt/rooset/etc/core-service to run core services
ENTRYPOINT [ "/opt/rooset/etc/frontend-server" ]
