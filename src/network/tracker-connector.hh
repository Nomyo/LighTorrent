#pragma once
#include <string>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace TrackerConnector
{

  static const std::string REQUEST_PREFIX = "GET ";
  static const std::string REQUEST_SUFFIX = " HTTP/1.1\r\n\r\n";

  class TrackerConnector
  {
  public:
    TrackerConnector(std::string hostName);
    int connect();

  private:
    std::string hostName_;
    bool opened_;
    int fd_;
    struct sockaddr_in servAddr_;
    struct hostent *server_;

  };
}
