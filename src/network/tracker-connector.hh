#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "url-parser.hh"
#include "tracker-info.hh"
#include "http-driver.hh"
#include "udp-driver.hh"
#include "torrent.hh"
#include "peer.hh"

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
    using Peer = Network::Peer;

  public:
    // Ctor & Dtor
    TrackerConnector(Torrent *t);
    ~TrackerConnector();

    // Actions
    std::vector<Peer> announce(const std::string& url);

  private:
    UrlParser urlParser_;
    TrackerInfo trackerInfo_;
    Torrent *torrent_;
  };
}
