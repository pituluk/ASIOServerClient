This repository now includes CMake build files and exposes the `ASIOServerClient` target.

Quick example to consume with FetchContent from another project:

```cmake
cmake_minimum_required(VERSION 3.16)
project(Example)

include(FetchContent)
FetchContent_Declare(
  ASIOServerClient
  GIT_REPOSITORY https://github.com/pituluk/ASIOServerClient.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(ASIOServerClient)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE ASIOServerClient::ASIOServerClient)
```

Notes:
- `BUILD_EXAMPLE` is OFF by default; set it ON if you want to build the example client.
- `ASIOSERVERCLIENT_ENABLE_SSL` controls optional OpenSSL linking.
