#pragma once

#include <iostream>
#include <string>

#include "tracker-connector.hh"
#include "torrent.hh"

namespace Network
{
  class TrackerDriver
  {
    using TrackerConnector = TrackerConnector::TrackerConnector;
    using Torrent = Network::Torrent;

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
