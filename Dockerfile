FROM debian:trixie AS builder

ENV VCPKG_ROOT=/build/vcpkg

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends -y \
    g++-11 build-essential ninja-build \
    clang-format git wget sudo \
    curl zip unzip tar pkg-config \
    libssl-dev cmake ca-certificates && \
    rm -rf /var/lib/apt/lists/*

WORKDIR "/build"

RUN git clone https://github.com/microsoft/vcpkg.git -n && \
    cd vcpkg && \
    git checkout 2023.11.20 && \
    ./bootstrap-vcpkg.sh

RUN /build/vcpkg/vcpkg install nlohmann-json gtest spdlog drogon cpp-httplib[openssl] argh

COPY cmake cmake
COPY console console
COPY source source
COPY test test
COPY CMakeLists.txt CMakeLists.txt

ENV VCPKG_ROOT=/build/vcpkg
RUN mkdir build && cd build && cmake .. && make

FROM debian:trixie-slim as runner

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends -y \
    ca-certificates && \
    rm -rf /var/lib/apt/lists/*

COPY --from=builder /build/build/bin /bin/

EXPOSE 5000

ENTRYPOINT ["./bin/funny", "--host", "0.0.0.0", "--port", "5000"]
