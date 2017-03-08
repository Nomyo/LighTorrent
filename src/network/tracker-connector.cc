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

  std::string TrackerConnector::announce(const std::string& url)
  {
    urlParser_.parseUrl(url);
    trackerInfo_.setTrackerInfo(urlParser_.getHost(), urlParser_.getPort());

    if (urlParser_.getProtocol() == UrlProtocol::Http)
    {
      HttpDriver httpDriver;
      httpDriver.sendRequest(urlParser_, trackerInfo_);
      return httpDriver.getResult();
    }
    else
    {
      UdpDriver udpDriver(torrent_, trackerInfo_);
      udpDriver.announce();
      return "";
    }

  }
}
