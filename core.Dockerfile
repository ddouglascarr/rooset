FROM debian:buster

RUN apt-get update && apt-get install -y \
    bmake \
    build-essential \
    libpq-dev \
    lighttpd \
    pmake \
    exim4 \
    postgresql-client \
    python-pip \
    sudo \
    wget

RUN pip install markdown2

ADD ./lfcore/ /opt/lfcore/
ADD ./etc/keep-alive /usr/local/bin/
ADD ./etc/core-services /usr/local/bin/
ADD ./etc/init-test-db /usr/local/bin/

WORKDIR /opt/lfcore
RUN bmake

ENV PGHOST="db"
ENV PGUSER="postgres"
ENV PGDATABASE="liquid_feedback"
# seconds before first execution
ENV CORE_SERVICES_INITIAL_DELAY="10"  
# seconds between runs
ENV CORE_SERVICES_REPEAT_DELAY="5"

ENTRYPOINT [ "/usr/local/bin/core-services" ]
