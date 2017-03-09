#pragma once

#include <iostream>
#include <netdb.h>
#include <strings.h>

namespace Network
{
  class TrackerInfo
  {
    public:
      // Ctor & Dtor
      TrackerInfo();
      TrackerInfo(const std::string& host, int port);
      ~TrackerInfo();

      void setTrackerInfo(const std::string& host, int port);

      // Getters
      const struct sockaddr_in& getServerAddress() const;
      const struct hostent *getServer() const;
      bool isResolved() const;

    private:
      struct sockaddr_in serverAddress_;
      struct hostent *server_;
  };
}
