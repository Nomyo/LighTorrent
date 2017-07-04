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
#include <thread>
#include <mutex>
#include <algorithm>

#include "network/tracker-connector.hh"
#include "network/peer.hh"
#include "file-manager.hh"
#include "torrent.hh"
#include "url-utils.hh"

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
    void dumpPeers() const;
    void addNewPeers(std::vector<Network::Peer> peers);

  private:
    void connectPeers();
    void updatePeers();
    void initiateHandshake(struct epoll_event *event, int fd);
    std::mutex eMutex_;
    std::thread updater_;
    std::vector<Network::Peer> waitingPeers_;
    std::map<int, Network::Peer> pendingPeers_;
    std::map<int, Network::Peer> connectedPeers_;
    Torrent *torrent_;
    FileManager fileManager_;
    int epfd_; // epoll fd
  };
}
