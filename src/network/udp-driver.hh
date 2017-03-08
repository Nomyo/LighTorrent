#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "tracker-info.hh"
#include "torrent.hh"
#include "../core/url-utils.hh"
#include "ip-address.hh"

namespace NetworkDriver
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
    using TrackerInfo = TrackerInfo::TrackerInfo;
    using Torrent = Network::Torrent;

    public:
      // Ctor & Dtor
      UdpDriver(Torrent *t);
      ~UdpDriver();

      int sendRequest(const TrackerInfo& trackerInfo);
      int tryConnect();
      int tryAnnounce(uint64_t connectionId, uint32_t transactionId);

    private:
      int fd_;
      Torrent *torrent_;
      TrackerInfo trackerInfo_;
  };

  struct connectionRequest createRequestAnnounce(int transactionId);
  struct announceRequest createAnnounceRequest(Network::Torrent *t, uint64_t connectionId,
                                               uint32_t transactionId);
}