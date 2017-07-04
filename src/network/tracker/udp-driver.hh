#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "core/torrent.hh"
#include "core/url-utils.hh"

#include "network/tracker/tracker.hh"
#include "network/peer.hh"
#include "network/utils.hh"

namespace Network
{
  struct connectionRequest
  {
    uint64_t connectionId;
    uint32_t action;
    uint32_t transactionId;
  };

  struct connectionResponse
  {
    uint32_t action;
    uint32_t transactionId;
    uint64_t connectionId;
  };

  struct announceRequest
  {
    uint64_t connectionId;
    uint32_t action;
    uint32_t transactionId;
    uint8_t info_hash[20];
    uint8_t peer_id[20];
    uint64_t downloaded;
    uint64_t left;
    uint64_t uploaded;
    uint32_t event;
    uint32_t ip;
    uint32_t key;
    uint32_t num_want;
    uint16_t port;
  };

  struct announceResponse
  {
    uint32_t action;
    uint32_t transactionId;
    uint32_t interval;
    uint32_t leechers;
    uint32_t seeders;
    uint8_t peer_infos[1500];
  };


  class UdpDriver
  {
    using Torrent = Core::Torrent;

    public:
      // Ctor & Dtor
      UdpDriver(Torrent *t, const Tracker& tracker);
      ~UdpDriver();

      std::vector<Network::Peer> announce();

    private:
      uint64_t tryConnect();
      std::vector<Peer> tryAnnounce(uint64_t connectionId);

      int fd_;
      Torrent *torrent_;
      Tracker tracker_;
  };

  struct connectionRequest createRequestAnnounce(int transactionId);
  struct announceRequest createAnnounceRequest(Core::Torrent *t, uint64_t connectionId);
  void setSocketTimeout(int fd);
}
