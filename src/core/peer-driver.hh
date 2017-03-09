#pragma once

#include <vector>
#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <map>

#include "../network/peer.hh"

namespace Core
{
  class PeerDriver
  {
    public:
      // Ctor & Dtor
      PeerDriver();
      PeerDriver(std::vector<Network::Peer> peers);
      ~PeerDriver();

      void startLeeching();
      void dumpPeers() const;

    private:
      std::vector<Network::Peer> peers_;
  };
}
