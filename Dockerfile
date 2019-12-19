FROM ubuntu:18.04

RUN apt-get update && \
    apt-get install -y --no-install-recommends  apt-transport-https \
                                                ca-certificates \
                                                gnupg \
                                                software-properties-common \
                                                wget


RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add -
RUN apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
RUN apt-get update
RUN apt-get install -y --no-install-recommends  cmake

RUN apt-get update && \
    apt-get install -y --no-install-recommends  clang-format \
                                                build-essential \
                                                gdb \
                                                git

RUN apt-get install -y --no-install-recommends  libcairo2-dev
RUN apt-get install -y --no-install-recommends  imagemagick
RUN apt-get install -y --no-install-recommends  libgraphicsmagick1-dev
RUN apt-get install -y --no-install-recommends  libpng-dev

WORKDIR /opt
RUN git clone --recurse-submodules https://github.com/cpp-io2d/P0267_RefImpl
WORKDIR /opt/P0267_RefImpl
RUN mkdir Debug
WORKDIR /opt/P0267_RefImpl/Debug
RUN cmake --config Debug "-DCMAKE_BUILD_TYPE=Debug" ..
RUN cmake --build .
RUN make install

ENV DISPLAY=host.docker.internal:0

WORKDIR /opt/route-planning
