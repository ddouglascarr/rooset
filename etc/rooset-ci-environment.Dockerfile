FROM eventstore/eventstore:release-4.0.0-rc1

RUN apt-get update && apt-get install -y \
  clang \
  clang-format \
  clang-tidy \
  cmake \
  git \
  maven \
  default-jdk \
  default-jre \
  libcurl4-openssl-dev \
  libboost-all-dev

# install nodejs 6.X
RUN curl -sL https://deb.nodesource.com/setup_6.x | bash - && \
  apt-get install -y nodejs

ENTRYPOINT ["/bin/bash"]
