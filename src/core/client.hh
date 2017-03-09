#pragma once

#include <iostream>
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
#include "../network/tracker-connector.hh"
#include "../network/url-parser.hh"

#include "../bencode/bencode-driver.hh"
#include "../bencode/bencode-utils.hh"
#include "../bencode/fwd.hh"

#include "../core/url-utils.hh"
#include "../core/tracker-driver.hh"

#include "torrent.hh"

using namespace BEncode;

namespace Core
{
  class Client
  {
  public:
    // Ctor & Dtor
    Client();
    Client(Torrent torrent);
    ~Client();

    void download(const std::string& filename);
    void getPeersFromBinary(const std::string& binaryPeers);
    void connectToPeers();
    void dumpPeers();

  private:
    // laters we would have a list of torrent
    // each torrent mapped to a list of peer
    std::vector<Network::Peer> peers_;
    Torrent torrent_;
  };

} // namespace Network
