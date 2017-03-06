#pragma once

#include <iostream>
#include <string>

#include "url-parser.hh"
#include "tracker-info.hh"
#include "http-driver.hh"
#include "udp-driver.hh"

namespace TrackerConnector
{
  class TrackerConnector
  {
    using UrlProtocol = UrlParser::urlProtocol;
    using UrlParser = UrlParser::UrlParser;
    using TrackerInfo = TrackerInfo::TrackerInfo;
    using HttpDriver = NetworkDriver::HttpDriver;
    using UdpDriver = NetworkDriver::UdpDriver;

  public:
    // Ctor & Dtor
    TrackerConnector();
    ~TrackerConnector();

    // Actions
    int sendRequest(const std::string& url);

    // Accessors
    std::string getResult() const;

  private:
    UrlParser urlParser_;
    TrackerInfo trackerInfo_;

    std::string result_;
  };
}
