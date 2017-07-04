#pragma once

#include <iostream>
#include <netdb.h>
#include <strings.h>

namespace Network
{
  class Tracker
  {
    public:
      // Ctor & Dtor
      Tracker();
      Tracker(const std::string& host, int port);
      ~Tracker();

      // Getters
      const struct sockaddr_in& getServerAddress() const;
      const struct hostent *getServer() const;
      bool isResolved() const;
      void setTracker(const std::string& host, int port);

    private:
      struct sockaddr_in serverAddress_;
      struct hostent *server_;
  };
}
