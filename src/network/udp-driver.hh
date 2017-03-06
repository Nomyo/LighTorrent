#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "tracker-info.hh"

namespace NetworkDriver
{
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
}
