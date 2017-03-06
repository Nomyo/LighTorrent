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
#include "http-driver.hh"

namespace TrackerConnector
{
  class TrackerConnector
  {
    using UrlProtocol = UrlParser::urlProtocol;
    using UrlParser = UrlParser::UrlParser;
    using TrackerInfo = TrackerInfo::TrackerInfo;
    using HttpDriver = NetworkDriver::HttpDriver;

  public:
    // Ctor & Dtor
    TrackerConnector();
    ~TrackerConnector();

    // Actions
    int sendRequest(const std::string& url);

    // Accessors
    std::string getResult() const;

  private:
    int createSocket();

    // Udp tracker related
    int requestUdpTracker();

    UrlParser urlParser_;
    TrackerInfo trackerInfo_;

    std::string result_;
  };
}
