#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../core/torrent.hh"

#include "url-parser.hh"
#include "tracker-info.hh"
#include "http-driver.hh"
#include "udp-driver.hh"
#include "peer.hh"

namespace Network
{
  class TrackerConnector
  {
    using Torrent = Core::Torrent;

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
