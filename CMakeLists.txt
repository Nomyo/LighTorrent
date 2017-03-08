cmake_minimum_required(VERSION 3.5)
project(LighTorrent)

#set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -O0 -Wall -Wextra")

set(

  SOURCE_FILES

  src/main.cpp
  src/core/url-utils.cc
  src/core/torrent.cc
  src/bencode/bencode-type.cc
  src/bencode/bencode-string.cc
  src/bencode/bencode-integer.cc
  src/bencode/bencode-list.cc
  src/bencode/bencode-dico.cc
  src/bencode/bencode-driver.cc
  src/network/tracker-connector.cc
  src/network/url-parser.cc
  src/network/http-driver.cc
  src/network/udp-driver.cc
  src/network/tracker-info.cc
  )


add_executable(LighTorrent ${SOURCE_FILES})
target_link_libraries(LighTorrent ssl)
target_link_libraries(LighTorrent crypto)
