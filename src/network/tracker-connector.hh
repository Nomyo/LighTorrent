#pragma once

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iomanip>

#include "url-parser.hh"

namespace TrackerConnector
{
  static const std::string REQUEST_PREFIX = "GET ";
  static const std::string REQUEST_SUFFIX = " HTTP/1.1\r\n";

  class TrackerConnector
  {

    using UrlParser = UrlParser::UrlParser;

  public:
    // Ctor & Dtor
    TrackerConnector();
    ~TrackerConnector();

    // Actions
    int sendRequest(std::string url);

    // Accessors
    std::string getResult();
    std::string getResultHeader();
    std::string getResultBody();

  private:
    int createSocket();
    int resolveHost(std::string host);
    int craftRequest(std::string url);
    int readResult();
    void formatResult(std::string& result);
    void deleteChunkInfo(std::string& s);

    int fd_;
    bool opened_;

    UrlParser urlParser_;

    struct sockaddr_in servAddr_;
    struct hostent *server_;

    //std::string result_;
    std::string resultHeader_;
    std::string resultBody_;
  };
}
