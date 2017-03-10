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
    int nbFailed = 0;
    int nbConnected = 0;
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
        nbConnected++;
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
        nbFailed++;
        continue;
      }
    }
    struct epoll_event *events = (struct epoll_event *)calloc(15, sizeof (struct epoll_event));

    ColorModifier colorRed(ColorCode::FG_RED);
    ColorModifier colorWhite(ColorCode::FG_DEFAULT);
    ColorModifier colorGreen(ColorCode::FG_GREEN);

    std::cout << "Watching " << nbWatched << " connections..." << std::endl;
    while (nbWatched > 0)
    {
      //std::cout << "---- before wait, watched = " << nbWatched << std::endl;
      int ndfs = epoll_wait(epfd, events, 15, -1);
      for (int i = 0; i < ndfs; i++)
      {
        if (events[i].events & EPOLLERR
            && events[i].events & EPOLLOUT)
        {
          std::string ip = "UNRESOLVED";
          auto find = peer_fd.find(events[i].data.fd);
          if (find != peer_fd.end())
            ip = find->second;
          std::cout << colorRed << "[" << ip << "]: connection lost "
            << "(" << events[i].events << ")"
            "  >  " << (events[i].events & EPOLLERR)
            << "  |  " << (events[i].events & EPOLLHUP)
            << "  |  " << (events[i].events & EPOLLRDHUP)
            << colorWhite << std::endl;
          epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
          close(events[i].data.fd);
          nbWatched--;
          nbFailed++;
          continue;
        }
        else if (events[i].events & EPOLLOUT)
        {
          events[i].events = EPOLLIN;
          epoll_ctl(epfd, EPOLL_CTL_MOD, events[i].data.fd, &events[i]);
          connectedPeers_.insert(*pendingPeers_.find(events[i].data.fd));
          pendingPeers_.erase(events[i].data.fd);
          auto connectedPeerIt = connectedPeers_.find(events[i].data.fd);
          if (connectedPeerIt != connectedPeers_.end())
          {
            long arg = fcntl(events[i].data.fd, F_GETFL, NULL);
            arg &= (~O_NONBLOCK);
            fcntl(events[i].data.fd, F_SETFL, arg);
            connectedPeerIt->second.setTorrent(torrent_);
            connectedPeerIt->second.setFd(events[i].data.fd);
            connectedPeerIt->second.tryHandshake();
          }
          continue;
        }
        else if (events[i].events & EPOLLIN)
        {
          auto connectedPeerIt = connectedPeers_.find(events[i].data.fd);
          if (connectedPeerIt != connectedPeers_.end())
          {
            struct sockaddr_in addr;
            socklen_t addr_size = sizeof (struct sockaddr_in);
            int res = getpeername(events[i].data.fd, (struct sockaddr *)&addr, &addr_size);
            if (res == -1)
              std::cout << "Error on getpeername " << strerror(errno) << std::endl;
            char *clientip = new char[20];
            strcpy(clientip, inet_ntoa(addr.sin_addr));


            connectedPeerIt->second.onReceive();
            epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
            close(events[i].data.fd);
            nbWatched--;
            if (connectedPeerIt->second.handshakeDone())
            {
              nbConnected++;
              std::cout << colorGreen << "[" << clientip << ":"
                << ntohs(addr.sin_port) << "]: handshake success! "
                << colorWhite << std::endl;
            }
            else
            {
              std::cout << colorRed << "[" << clientip << ":"
                << ntohs(addr.sin_port) << "]: handshake failed "
                << colorWhite << std::endl;
              nbFailed++;
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
    std::cout << "nbConnected = " << nbConnected << std::endl;
    std::cout << "nbFailed: = " << nbFailed << std::endl;
  }

  void PeerDriver::dumpPeers() const
  {
    for (auto peer: peers_)
      peer.dump();
  }
}
