#include "tracker-driver.hh"

namespace Network
{
  TrackerDriver::TrackerDriver(Torrent *t)
    : torrent_(t)
  { }

  TrackerDriver::~TrackerDriver()
  { }


  void TrackerDriver::createConnectors()
  {
    auto announces = torrent_->getAnnounceList();
    for (auto announce : announces)
    {


    }
  }

  void TrackerDriver::announces() const
  {

  }
}
