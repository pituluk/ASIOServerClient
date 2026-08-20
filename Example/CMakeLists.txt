cmake_minimum_required(VERSION 3.16)

include(FetchContent)

if(NOT BUILD_EXAMPLE)
  return()
endif()

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG        v5.0.0
)
FetchContent_MakeAvailable(ftxui)

add_executable(Client
  TestClient.cpp
  ClientImpl.cpp
  ChatGUI.cpp
)
target_include_directories(Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Client PRIVATE ASIOServerClient ftxui::component)

add_executable(Server
  TestServer.cpp
  ServerImpl.cpp
)
target_include_directories(Server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Server PRIVATE ASIOServerClient)
