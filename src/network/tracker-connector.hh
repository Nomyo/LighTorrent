#pragma once

#include <iostream>
#include <string>

#include "url-parser.hh"
#include "tracker-info.hh"
#include "http-driver.hh"
#include "udp-driver.hh"
#include "torrent.hh"

namespace TrackerConnector
{
  class TrackerConnector
  {
    using UrlProtocol = UrlParser::urlProtocol;
    using UrlParser = UrlParser::UrlParser;
    using TrackerInfo = TrackerInfo::TrackerInfo;
    using HttpDriver = NetworkDriver::HttpDriver;
    using UdpDriver = NetworkDriver::UdpDriver;
    using Torrent = Network::Torrent;

  public:
    // Ctor & Dtor
    TrackerConnector(Torrent *t);
    ~TrackerConnector();

    // Actions
    std::string announce(const std::string& url);

    // Accessors
    std::string getResult() const;

  private:
    UrlParser urlParser_;
    TrackerInfo trackerInfo_;
    Torrent *torrent_;

    std::string result_;
  };
}
