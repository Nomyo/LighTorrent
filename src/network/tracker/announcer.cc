#include "announcer.hh"

namespace Network
{
  Announcer::Announcer(Torrent *t)
  {
    torrent_ = t;
  }

  Announcer::~Announcer()
  {}

  std::vector<Network::Peer> Announcer::announce()
  {
    std::cout << "Announcing to URL <" << torrent_->getUrlGenerated() << ">"
              << std::endl;
    urlParser_.parseUrl(torrent_->getUrlGenerated());
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
