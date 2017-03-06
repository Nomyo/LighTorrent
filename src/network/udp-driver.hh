#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "tracker-info.hh"

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


  class UdpDriver
  {
    using TrackerInfo = TrackerInfo::TrackerInfo;

    public:
      // Ctor & Dtor
      UdpDriver();
      ~UdpDriver();

      int sendRequest(const TrackerInfo& trackerInfo);

    private:
      int fd_;

  };

  struct connectionRequest createRequestAnnounce(int transactionId);
}
