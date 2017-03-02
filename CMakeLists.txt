cmake_minimum_required(VERSION 3.5)
project(LighTorrent)

set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(

  SOURCE_FILES

  src/main.cpp
  src/bencode/bencode-type.cc
  src/bencode/bencode-string.cc
  src/bencode/bencode-integer.cc
  src/bencode/bencode-driver.cc
  )

add_executable(LighTorrent ${SOURCE_FILES})
