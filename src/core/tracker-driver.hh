#pragma once

#include <iostream>
#include <string>

#include "../network/tracker-connector.hh"

#include "torrent.hh"

namespace Core
{
  class TrackerDriver
  {
    using TrackerConnector = Network::TrackerConnector;

  public:
    // Ctor & Dtor
    TrackerDriver(Torrent *t);
    ~TrackerDriver();

    void createConnectors();
    void announces() const;

  private:
    Torrent *torrent_;
    std::vector<TrackerConnector> trackConnectors_;
  };
}
