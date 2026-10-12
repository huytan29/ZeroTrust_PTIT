FROM debian:trixie-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends\
        cmake \
        g++ \
        make \
        libcpp-httplib-dev

WORKDIR /app

COPY CMakeLists.txt main.cpp ./

RUN cmake -S . -B build \
    && cmake --build build

EXPOSE 8020

CMD ["./build/ztas"]