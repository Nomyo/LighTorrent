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

  int TrackerConnector::sendRequest(const std::string& url)
  {
    urlParser_.parseUrl(url);
    trackerInfo_.setTrackerInfo(urlParser_.getHost(), urlParser_.getPort());

    if (urlParser_.getProtocol() == UrlProtocol::Http)
    {
      HttpDriver httpDriver;
      httpDriver.sendRequest(urlParser_, trackerInfo_);
      result_ = httpDriver.getResult();
      return 1;
    }
    else
    {
      UdpDriver udpDriver(torrent_);
      udpDriver.sendRequest(trackerInfo_);
      return 1;
    }
  }

  std::string TrackerConnector::getResult() const
  {
    return result_;
  }
}
