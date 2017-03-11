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
  {
    if (updater_.joinable())
      updater_.join();
  }

  void PeerDriver::startLeeching()
  {
    connectPeers();
    //std::cout << "pending peers: " << pendingPeers_.size() << std::endl;
    if (pendingPeers_.size() != 0)
      updater_ = std::thread(&PeerDriver::updatePeers, this);

    while (true)
    {
      if (pendingPeers_.size() == 0)
      {
        Core::URLUtils url;
        std::string urlGenerated = url.generateURL(*torrent_);
        Network::TrackerConnector tc(torrent_);
        addNewPeers(tc.announce(urlGenerated));
        connectPeers();
      }
    }
    std::cout << "Finished!" << std::endl;
  }

  void PeerDriver::connectPeers()
  {
    // For all peers waiting
    for (auto peer : waitingPeers_)
    {
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);
      fcntl(sockfd, F_SETFL, O_NONBLOCK);

      struct sockaddr_in remoteaddr;
      remoteaddr.sin_family = AF_INET;
      remoteaddr.sin_addr.s_addr = inet_addr(peer.getIp().c_str());
      remoteaddr.sin_port = htons(peer.getPort());

      // we try to connect
      int res = connect(sockfd, (struct sockaddr *)&remoteaddr, sizeof (remoteaddr));

      // if the connection did not fail immediatly, we put the peer in "pending" state
      if (res != -1 || errno == EINPROGRESS)
      {
        eMutex_.lock();
        pendingPeers_.insert(std::make_pair(sockfd, peer));
        struct epoll_event ev;
        bzero(&ev, sizeof (struct epoll_event));
        ev.events = EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
        ev.data.fd = sockfd;
        epoll_ctl(epfd_, EPOLL_CTL_ADD, sockfd, &ev);
        eMutex_.unlock();
      }
      else
        close(sockfd);
    }
    waitingPeers_.clear();
  }

  void PeerDriver::updatePeers()
  {
    std::cout << "Started the updatePeers thread..." << std::endl;
    int handshakeSuccess = 0;
    struct epoll_event *events = (struct epoll_event *)calloc(150, sizeof (struct epoll_event));

    while (pendingPeers_.size() || connectedPeers_.size())
    {
      std::cout << pendingPeers_.size() << " | " << connectedPeers_.size()
        << " (" << handshakeSuccess << ")" << std::endl;
      int ndfs = epoll_wait(epfd_, events, 150, 20000);
      for (int i = 0; i < ndfs; i++)
      {
        // If the peer did not even connect (connection error)
        if (events[i].events & EPOLLERR && events[i].events & EPOLLOUT)
        {
          auto peer = pendingPeers_.find(events[i].data.fd);
          if (peer != pendingPeers_.end())
          {
            eMutex_.lock();
            epoll_ctl(epfd_, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
            pendingPeers_.erase(peer->first);
            eMutex_.unlock();
            close(events[i].data.fd);
          }
        }

        // If the peer connected successfully, try to handshake
        else if (events[i].events & EPOLLOUT)
          initiateHandshake(&events[i], events[i].data.fd);

        // We got a message from the peer
        else if (events[i].events & EPOLLIN)
        {
          auto connectedPeerIt = connectedPeers_.find(events[i].data.fd);
          if (connectedPeerIt != connectedPeers_.end())
          {
            connectedPeerIt->second.onReceive();
            eMutex_.lock();
            handshakeSuccess++;
            connectedPeers_.erase(connectedPeerIt->first);
            close(events[i].data.fd);
            eMutex_.unlock();
          }
        }
      }
    }
    std::cout << "No more peers to update, stopping thread..." << std::endl;
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

  void PeerDriver::addNewPeers(std::vector<Network::Peer> peers)
  {
    waitingPeers_.insert(waitingPeers_.end(), peers.begin(), peers.end());
  }

  void PeerDriver::initiateHandshake(struct epoll_event *event, int fd)
  {
    eMutex_.lock();
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
      //long arg = fcntl(fd, F_GETFL, NULL);
      //arg &= (~O_NONBLOCK);
      //fcntl(fd, F_SETFL, arg);
      connectedPeerIt->second.setTorrent(torrent_);
      connectedPeerIt->second.setFd(fd);
      connectedPeerIt->second.tryHandshake();
    }
    eMutex_.unlock();
  }
}
