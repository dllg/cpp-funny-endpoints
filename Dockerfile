FROM alpine:latest as builder

VOLUME "/conan-build"
WORKDIR "/conan-build"

RUN apk update && \
    apk upgrade && \
    apk --update add \
    gcc \
    g++ \
    build-base \
    cmake \
    bash \
    libstdc++ \
    cppcheck \
    autoconf \
    linux-headers \
    py-pip && \
    pip install conan && \
    rm -rf /var/cache/apk/*

COPY conanfile.txt conanfile.txt

RUN conan install . -s cppstd=17 -s compiler=gcc -s compiler.libcxx=libstdc++11 --build

WORKDIR "/build"

COPY . .

RUN mkdir -p build-linux && \
    cd build-linux && \
    conan install .. -s cppstd=17 -s compiler=gcc -s compiler.libcxx=libstdc++11 -o *:shared=False && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    cmake --build .

FROM alpine:latest as runner

COPY --from=builder /build/build-linux/bin /build/build-linux/test/bin /bin/

RUN apk update && \
    apk upgrade && \
    apk --update add \
    libstdc++

CMD ["./bin/funny"]
