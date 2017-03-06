#pragma once

#include <iostream>
#include <netdb.h>
#include <strings.h>

namespace TrackerInfo
{
  class TrackerInfo
  {
    public:
      // Ctor & Dtor
      TrackerInfo(const std::string& host, int port);
      ~TrackerInfo();

      const struct sockaddr_in& getServerAddress() const;
      const struct hostent *getServer() const;

    private:
      struct sockaddr_in serverAddress_;
      struct hostent *server_;
  };
}