#pragma once

#include <iostream>
#include <string>

#include "network/tracker/announcer.hh"

#include "core/torrent.hh"

namespace Core
{
  class TrackerDriver
  {
    using Announcer = Network::Announcer;

  public:
    // Ctor & Dtor
    TrackerDriver(Torrent *t);
    ~TrackerDriver();

    void createConnectors();
    void announces() const;

  private:
    Torrent *torrent_;
    std::vector<Announcer> announcers_;
  };
}
