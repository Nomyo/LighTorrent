#pragma once
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

namespace TrackerConnector
{

  static const std::string REQUEST_PREFIX = "GET ";
  static const std::string REQUEST_SUFFIX = " HTTP/1.1\r\n";

  class TrackerConnector
  {
  public:
    TrackerConnector();
    ~TrackerConnector();
    int connect();

  private:
    int createSocket();

    int fd_;
    bool opened_;
    std::string hostName_;
    struct sockaddr_in servAddr_;
    struct hostent *server_;

  };
}
