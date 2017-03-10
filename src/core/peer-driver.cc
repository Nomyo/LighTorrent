#include "peer-driver.hh"

namespace Core
{
  PeerDriver::PeerDriver()
  { }

  PeerDriver::PeerDriver(std::vector<Network::Peer> peers, Torrent *torrent)
    : torrent_(torrent)
  {
    waitingPeers_ = peers;
    epfd_ = epoll_create(350);
  }

  PeerDriver::~PeerDriver()
  { }

  void PeerDriver::startLeeching()
  {
    connectPeers();

    struct epoll_event *events = (struct epoll_event *)calloc(150, sizeof (struct epoll_event));

    ColorModifier colorRed(ColorCode::FG_RED);
    ColorModifier colorWhite(ColorCode::FG_DEFAULT);
    ColorModifier colorGreen(ColorCode::FG_GREEN);

    while (true)
    {
      int ndfs = epoll_wait(epfd_, events, 150, 20000);
      for (int i = 0; i < ndfs; i++)
      {
        if (events[i].events & EPOLLERR
            && events[i].events & EPOLLOUT)
        {
          auto peer = pendingPeers_.find(events[i].data.fd);
          if (peer != pendingPeers_.end())
          {
            std::cout << colorRed << "[" << peer->second.getIp() << "]: connection lost "
              << colorWhite << std::endl;
            epoll_ctl(epfd_, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
            close(events[i].data.fd);
          }
        }
        else if (events[i].events & EPOLLOUT)
        {
          initiateHandshake(&events[i], events[i].data.fd);
        }
        else if (events[i].events & EPOLLIN)
        {
          auto connectedPeerIt = connectedPeers_.find(events[i].data.fd);
          if (connectedPeerIt != connectedPeers_.end())
          {
            connectedPeerIt->second.onReceive();
            epoll_ctl(epfd_, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
            close(events[i].data.fd);
            if (connectedPeerIt->second.handshakeDone())
            {
              std::cout << colorGreen << "[" << connectedPeerIt->second.getIp()
                << "]: handshake success! "
                << colorWhite << std::endl;
            }
            else
            {
              std::cout << colorRed << "[" << connectedPeerIt->second.getIp()
                << "]: handshake failed "
                << colorWhite << std::endl;
            }
          }
          else
            std::cout << "Error on getting connected peer in map..." << std::endl;
        }
        else
          std::cout << "Unexpected condition..." << std::endl;
      }
    }
    std::cout << "Finished!" << std::endl;
  }

  void PeerDriver::connectPeers()
  {
    for (auto peer : waitingPeers_)
    {
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);
      fcntl(sockfd, F_SETFL, O_NONBLOCK);

      struct sockaddr_in remoteaddr;
      remoteaddr.sin_family = AF_INET;
      remoteaddr.sin_addr.s_addr = inet_addr(peer.getIp().c_str());
      remoteaddr.sin_port = htons(peer.getPort());

      int res = connect(sockfd, (struct sockaddr *)&remoteaddr, sizeof (remoteaddr));
      if (res != -1 || errno == EINPROGRESS)
      {
        pendingPeers_.insert(std::make_pair(sockfd, peer));
        struct epoll_event ev;
        bzero(&ev, sizeof (struct epoll_event));
        ev.events = EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
        ev.data.fd = sockfd;
        epoll_ctl(epfd_, EPOLL_CTL_ADD, sockfd, &ev);
      }
      else
        close(sockfd);
    }
    waitingPeers_.clear();
  }

  void PeerDriver::updatePeers()
  {

  }

  void PeerDriver::dumpPeers() const
  {
    std::cout << "pending peers:" << std::endl;
    for (auto peer: pendingPeers_)
      peer.second.dump();
    std::cout << "connected peers:" << std::endl;
    for (auto peer: connectedPeers_)
      peer.second.dump();
  }

  void PeerDriver::initiateHandshake(struct epoll_event *event, int fd)
  {
    event->events = EPOLLIN;
    epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, event);
    auto pendingPeerIt = pendingPeers_.find(fd);
    if (pendingPeerIt != pendingPeers_.end())
    {
      connectedPeers_.insert(std::make_pair(pendingPeerIt->first, pendingPeerIt->second));
      pendingPeers_.erase(pendingPeerIt);
    }
    auto connectedPeerIt = connectedPeers_.find(fd);
    if (connectedPeerIt != connectedPeers_.end())
    {
      long arg = fcntl(fd, F_GETFL, NULL);
      arg &= (~O_NONBLOCK);
      fcntl(fd, F_SETFL, arg);
      connectedPeerIt->second.setTorrent(torrent_);
      connectedPeerIt->second.setFd(fd);
      connectedPeerIt->second.tryHandshake();
    }
  }
}
