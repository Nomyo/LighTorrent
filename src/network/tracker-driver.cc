#include "tracker-driver.hh"

namespace Network
{
  TrackerDriver::TrackerDriver(Torrent *t)
    : torrent_(t)
  { }

  TrackerDriver::~TrackerDriver()
  { }

}
