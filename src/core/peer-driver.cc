#include "peer-driver.hh"

namespace Core
{
  PeerDriver::PeerDriver()
  { }

  PeerDriver::PeerDriver(std::vector<Network::Peer> peers, Torrent *torrent)
    : peers_(peers)
    , torrent_(torrent)
  { }

  PeerDriver::~PeerDriver()
  { }

  void PeerDriver::startLeeching()
  {
    std::map<int, std::string> peer_fd;
    int nbWatched = 0;
    int epfd = epoll_create(150);
    for (auto peer : peers_)
    {
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);
      pendingPeers_.insert(std::make_pair(sockfd, peer));
      fcntl(sockfd, F_SETFL, O_NONBLOCK);

      struct sockaddr_in remoteaddr;
      remoteaddr.sin_family = AF_INET;
      remoteaddr.sin_addr.s_addr = inet_addr(peer.getIp().c_str());
      remoteaddr.sin_port = htons(peer.getPort());

      int res = connect(sockfd, (struct sockaddr *)&remoteaddr, sizeof (remoteaddr));
      if (res == 0)
      {
        std::cout << "Connected to " << peer.getIp() << ":" << peer.getPort() << std::endl;
        close(sockfd);
        continue;
      }
      else if (errno == EINPROGRESS)
      {
        nbWatched++;
        peer_fd.insert(std::make_pair(sockfd, peer.getIp() + ":" + std::to_string(peer.getPort())));
        struct epoll_event ev;
        bzero(&ev, sizeof (struct epoll_event));
        ev.events = EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
        ev.data.fd = sockfd;
        epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
        continue;
      }
      else
      {
        std::cout << "Error while trying to connect to " << peer.getIp() << ":" << peer.getPort() << std::endl;
        close(sockfd);
        continue;
      }
    }
    struct epoll_event *events = (struct epoll_event *)calloc(15, sizeof (struct epoll_event));

    while (nbWatched > 0)
    {
      int ndfs = epoll_wait(epfd, events, 15, -1);
      for (int i = 0; i < ndfs; i++)
      {
        nbWatched--;
        if (events[i].events & EPOLLERR
            && events[i].events & EPOLLOUT)
        {
          std::string ip = "UNRESOLVED";
          auto find = peer_fd.find(events[i].data.fd);
          if (find != peer_fd.end())
            ip = find->second;
          std::cout << "Socket " << ip << " connection lost "
            << "(" << events[i].events << ")"
            "  >  " << (events[i].events & EPOLLERR)
            << "  |  " << (events[i].events & EPOLLHUP)
            << "  |  " << (events[i].events & EPOLLRDHUP)
            << std::endl;
          epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
          close(events[i].data.fd);
          continue;
        }
        else if (events[i].events & EPOLLOUT)
        {
          struct sockaddr_in addr;
          socklen_t addr_size = sizeof (struct sockaddr_in);
          int res = getpeername(events[i].data.fd, (struct sockaddr *)&addr, &addr_size);
          if (res == -1)
            std::cout << "Error on getpeername " << strerror(errno) << std::endl;
          char *clientip = new char[20];
          strcpy(clientip, inet_ntoa(addr.sin_addr));

          std::cout << "Socket " << clientip << ":" << ntohs(addr.sin_port) << " is connected! " << std::endl;

	  // TEST HANDSHAKE
	  connectedPeers_.insert(*pendingPeers_.find(events[i].data.fd));
	  pendingPeers_.erase(events[i].data.fd);
	  auto connectedPeerIt = connectedPeers_.find(events[i].data.fd);
	  if (connectedPeerIt != connectedPeers_.end())
	  {
	    long arg = fcntl(events[i].data.fd, F_GETFL, NULL);
	    arg &= (~O_NONBLOCK);
	    fcntl(events[i].data.fd, F_SETFL, arg);

	    struct timeval tv;
	    tv.tv_sec = 0;
	    tv.tv_usec = 500000;

	    if (setsockopt(events[i].data.fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv)) < 0)
	      std::cout << "failed timeout" << std::endl;

	    connectedPeerIt->second.setTorrent(torrent_);
	    connectedPeerIt->second.setFd(events[i].data.fd);
	    connectedPeerIt->second.tryHandshake();
	    connectedPeerIt->second.onReceive();
	  }

          // END TEST HANSHAKE

	  epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
	  close(events[i].data.fd);
          continue;
        }
        else
          std::cout << "Unexpected condition..." << std::endl;
      }
    }

    std::cout << "Finished!" << std::endl;
  }

  void PeerDriver::dumpPeers() const
  {
    for (auto peer: peers_)
      peer.dump();
  }
}
