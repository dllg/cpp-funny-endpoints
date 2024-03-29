![CI](https://github.com/dllg/cpp-funny-endpoints/workflows/CI/badge.svg)

# cpp-funny-endpoints
Simple C++ application that interacts with different open api's to create text that you can receive on GET endpoints. See [swagger.yml](swagger.yml)

Uses [vcpkg](https://vcpkg.io/) to handle dependencies and [cmake](https://cmake.org/) build system. It uses [nlohmann_json](https://github.com/nlohmann/json) for json, [drogon](https://drogon.io) for http server part, [cpr](https://github.com/whoshuu/cpr) for http client part, [spdlog](https://github.com/gabime/spdlog) for logging and [gtest](https://github.com/google/googletest) for testing.

## Testing

Testing is done using [gtest](https://github.com/google/googletest). Testing of the classes [Funny](./source/Funny.h) and [Service](./source/Service.h) are done using dependency injection. Mocking of the client and server classes are done using [googlemock](https://github.com/google/googletest/blob/master/docs/gmock_cook_book.md).


## Building locally
### 1. Setup dependencies using vcpkg
- Install [vcpkg](https://vcpkg.io/en/getting-started)
```
$ git clone ssh://git@github.com/microsoft/vcpkg
$ cd vcpkg
$ ./bootstrap-vcpkg.sh
```

- Install dependencies
```
$ ./vcpkg install nlohmann_json gtest spdlog drogon cpp-httplib
```

- set VCPKG_ROOT environment variable
```
$ export VCPKG_ROOT=/path/to/vcpkg
```

Create a build directory if you haven't already and initailize the dependencies:
```bash
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
cd ..
```

### 2. Building
```bash
cmake --build build
```

### 3. Running
```bash
./build/bin/funny
```

### 4. Running the tests
The tests are automatically run after each build. You can run them by invoking:
```bash
./build/bin/funnytests
```

## Building for docker
### 1. Building the image
```bash
docker-compose build
```

### 2. Running the container
```bash
docker-compose up -d
```

### 3. Running the tests
The tests are also run after building in docker. To run the tests in docker invoke:
```bash
docker run cpp-funny-endpoints funnytests
```

## Running in kubernetes
See [k8s/README.md](./k8s/README.md).

## Examples of using it
### Get an advice
```bash
$ curl http://localhost:5000/v1/advice
{"message":"If you don't want something to be public, don't post it on the Internet."}
```

### Get a Chuck Norris Joke
```bash
$ curl http://localhost:5000/v1/chucknorris
{"message":"there is no use crying over spilled milk, unless its Chuck Norris' milk because then your gonna die"}
```

### Get a dad joke
```bash
$ curl http://localhost:5000/v1/dadjoke
{"message":"What do you call an eagle who can play the piano? Talonted!"}
```

### Get a random message
```bash
$ curl http://localhost:5000/v1/random
{"message":"Advice: When painting a room, preparation is key. The actual painting should account for about 40% of the work."}
```
