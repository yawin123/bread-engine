cmake_minimum_required(VERSION 2.8.0)
set(CMAKE_CXX_FLAGS "-O3 -std=c++20")
project(BreadEngine)
add_library(
  BreadEngine
  SHARED
  src/core/components.cpp
  src/core/context.cpp
  src/system/render.cpp
  src/components/transform.cpp
  src/components/sprite.cpp
)

target_include_directories(BreadEngine PUBLIC include/)
find_library(SDL2_LIBRARY SDL2)
target_link_libraries(
  BreadEngine
  PUBLIC
  ${SDL2_LIBRARY}
  SDL2main
  SDL2_image
  SDL2_mixer
  SDL2_net
  SDL2_ttf
)
