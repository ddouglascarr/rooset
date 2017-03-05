FROM ddouglascarr/rooset-ci-environment

ADD . /opt/rooset
WORKDIR /opt/rooset
RUN git submodule update --init --recursive
RUN ./configure
ENTRYPOINT ["/bin/bash"]
