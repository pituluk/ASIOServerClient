cmake_minimum_required(VERSION 3.16)

if(NOT BUILD_EXAMPLE)
  return()
endif()

file(GLOB EXAMPLE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(Example ${EXAMPLE_SOURCES})
target_include_directories(Example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Example PRIVATE ASIOServerClient)
