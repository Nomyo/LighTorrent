#include "announcer.hh"

namespace Network
{
  Announcer::Announcer(Torrent *t)
  {
    torrent_ = t;
  }

  Announcer::~Announcer()
  {}

  std::vector<Network::Peer> Announcer::announce(const std::string& url)
  {
    std::cout << "Announcing to URL <" << url << ">" << std::endl;
    urlParser_.parseUrl(url);
    tracker_.setTracker(urlParser_.getHost(), urlParser_.getPort());

    if (urlParser_.getProtocol() == UrlProtocol::Http)
    {
      HttpDriver httpDriver;
      return httpDriver.announce(urlParser_, tracker_);
    }
    else
    {
      UdpDriver udpDriver(torrent_, tracker_);
      return udpDriver.announce();
    }

  }
}
