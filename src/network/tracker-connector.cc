#include <string.h>
#include "tracker-connector.hh"

namespace TrackerConnector
{
  TrackerConnector::TrackerConnector(Torrent *t)
  {
    torrent_ = t;
  }

  TrackerConnector::~TrackerConnector()
  {}

  std::list<Network::Peer> TrackerConnector::announce(const std::string& url)
  {
    urlParser_.parseUrl(url);
    trackerInfo_.setTrackerInfo(urlParser_.getHost(), urlParser_.getPort());

    if (urlParser_.getProtocol() == UrlProtocol::Http)
    {
      HttpDriver httpDriver;
      return httpDriver.announce(urlParser_, trackerInfo_);
    }
    else
    {
      UdpDriver udpDriver(torrent_, trackerInfo_);
      return udpDriver.announce();
    }

  }
}
