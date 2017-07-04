cmake_minimum_required(VERSION 3.5)
project(LighTorrent)

#set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -g -O0 -Wall -Wextra")

include_directories(src/)

set(

  SOURCE_FILES

  src/main.cpp
  src/core/url-utils.cc
  src/core/tracker-driver.cc
  src/core/torrent.cc
  src/core/torrent-manager.cc
  src/core/peer-driver.cc
  src/core/message-builder.cc
  src/core/file-manager.cc
  src/core/blocks.cc
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
  src/network/tracker.cc
  src/network/peer.cc
  src/network/utils.cc
  )


add_executable(LighTorrent ${SOURCE_FILES})
target_link_libraries(LighTorrent ssl)
target_link_libraries(LighTorrent crypto)
