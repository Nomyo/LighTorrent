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
    int sendRequest(std::string url);
    std::string getResult();

  private:
    int createSocket();
    int resolveHost(std::string host);
    int craftRequest(std::string url);
    int readResult();
    void parseUrl(std::string url);

    int fd_;
    bool opened_;
    std::string host_;
    std::string request_;
    int port_;
    struct sockaddr_in servAddr_;
    struct hostent *server_;
    std::string result_;

    std::string result_header_;
    std::string result_body_;
  };
}
