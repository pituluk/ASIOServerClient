cmake_minimum_required(VERSION 3.16)
project(ASIOServerClient VERSION 0.1.0 LANGUAGES CXX)

option(BUILD_EXAMPLE "Build example client" OFF)
option(ASIOSERVERCLIENT_ENABLE_SSL "Enable SSL support" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(asio)

file(GLOB ASIOSERVER_SOURCES "${CMAKE_SOURCE_DIR}/src/ASIOServerClient/*.cpp")

add_library(ASIOServerClient ${ASIOSERVER_SOURCES})

# Provide a namespaced alias so FetchContent consumers can link using
# `ASIOServerClient::ASIOServerClient`.
add_library(ASIOServerClient::ASIOServerClient ALIAS ASIOServerClient)

target_compile_features(ASIOServerClient PUBLIC cxx_std_20)

target_include_directories(ASIOServerClient PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if(DEFINED asio_SOURCE_DIR)
  target_include_directories(ASIOServerClient PUBLIC
    $<BUILD_INTERFACE:${asio_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/asio>
  )
endif()

if(ASIOSERVERCLIENT_ENABLE_SSL)
  find_package(OpenSSL QUIET)
  if(OpenSSL_FOUND)
    target_link_libraries(ASIOServerClient PUBLIC OpenSSL::SSL OpenSSL::Crypto)
    target_compile_definitions(ASIOServerClient PUBLIC ASIOSERVERCLIENT_HAVE_OPENSSL=1)
  else()
    message(STATUS "OpenSSL not found. If you want OpenSSL support set OPENSSL_ROOT_DIR to your OpenSSL installation and re-run CMake.")
  endif()
endif()

install(TARGETS ASIOServerClient EXPORT ASIOServerClientTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(EXPORT ASIOServerClientTargets FILE ASIOServerClientTargets.cmake NAMESPACE ASIOServerClient:: DESTINATION lib/cmake/ASIOServerClient)

# Install public headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)

if(BUILD_EXAMPLE AND EXISTS "${CMAKE_SOURCE_DIR}/Example/CMakeLists.txt")
  add_subdirectory(Example)
endif()
