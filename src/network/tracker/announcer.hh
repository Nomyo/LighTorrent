#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "core/torrent.hh"

#include "network/url-parser.hh"
#include "network/tracker/tracker.hh"
#include "network/tracker/http-driver.hh"
#include "network/tracker/udp-driver.hh"
#include "network/peer.hh"

namespace Network
{
  class Announcer
  {
    using Torrent = Core::Torrent;

  public:
    // Ctor & Dtor
    Announcer(Torrent *t);
    ~Announcer();

    // Actions
    std::vector<Peer> announce(const std::string& url);

  private:
    UrlParser urlParser_;
    Tracker tracker_;
    Torrent *torrent_;
  };
}
