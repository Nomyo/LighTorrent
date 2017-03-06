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
#include "tracker-info.hh"

namespace TrackerConnector
{
  class TrackerConnector
  {
    using UrlProtocol = UrlParser::urlProtocol;
    using UrlParser = UrlParser::UrlParser;
    using TrackerInfo = TrackerInfo::TrackerInfo;

  public:
    // Ctor & Dtor
    TrackerConnector();
    ~TrackerConnector();

    // Actions
    int sendRequest(const std::string& url);

    // Accessors
    std::string getResult() const;
    std::string getResultHeader() const;
    std::string getResultBody() const;

  private:
    int createSocket();
    //int resolveHost(const std::string& host);

    // Http tracker related
    int requestHttpTracker();
    int craftRequest() const;
    int readResult();

    // Udp tracker related
    int requestUdpTracker();

    void formatResult(std::string& result);
    void deleteChunkInfo(std::string& s);

    int fd_;
    bool opened_;

    UrlParser urlParser_;
    TrackerInfo trackerInfo_;

    std::string resultHeader_;
    std::string resultBody_;
  };
}
