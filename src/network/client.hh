#pragma once

#include "peer.hh"
#include "torrent.hh"
#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

namespace Network
{
  class Client
  {
  public:
    // Ctor & Dtor
    Client();
    Client(Torrent torrent);
    ~Client();

    void getPeersFromBinary(const std::string& binaryPeers);
    void connectToPeers();
    void dumpPeers();

  private:
    std::vector<Peer> peers_;
    Torrent torrent_;
  };

} // namespace Network
