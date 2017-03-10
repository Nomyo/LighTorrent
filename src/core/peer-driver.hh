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
#include "torrent.hh"
#include "color.hh"

namespace Core
{
  class PeerDriver
  {
  public:
    // Ctor & Dtor
    PeerDriver();
    PeerDriver(std::vector<Network::Peer> peers, Torrent *torrent);
    ~PeerDriver();

    void startLeeching();
    void connectPeers();
    void updatePeers();
    void dumpPeers() const;

  private:
    void initiateHandshake(struct epoll_event *event, int fd);

    std::vector<Network::Peer> waitingPeers_;
    std::map<int, Network::Peer> pendingPeers_;
    std::map<int, Network::Peer> connectedPeers_;
    Torrent *torrent_;
    int epfd_; // epoll fd
  };
}
