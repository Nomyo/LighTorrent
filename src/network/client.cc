#include "client.hh"

namespace Network
{

  Client::Client()
  { }

  Client::Client(Torrent torrent)
    : torrent_(torrent)
  { }

  Client::~Client()
  { }

  void Client::download(const std::string& filename)
  {
    BEncodeDriver driver;
    auto decodedFile = driver.bDecodeFile(filename);

    if (!decodedFile)
    {
      std::cerr << filename << " is not a valid torrent file" << std::endl;
      return;
    }

    torrent_ = (getType<BType_ptr, BDico>(decodedFile));

    Core::URLUtils url;
    std::string urlGenerated = url.generateURL(torrent_);
    UrlParser::UrlParser up(urlGenerated);
    up.dump();
    std::cout << std::endl;

    TrackerDriver trackDriver(&torrent_);
    trackDriver.createConnectors();
    trackDriver.announces();

    TrackerConnector::TrackerConnector tc(&torrent_);
    peers_ = tc.announce(urlGenerated);
    dumpPeers();
    connectToPeers();
  }

  void Client::connectToPeers()
  {
    std::map<int, std::string> peer_fd;
    int nbWatched = 0;
    int epfd = epoll_create(150);
    for (auto peer : peers_)
    {
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

  void Client::dumpPeers()
  {
    for (auto i : peers_)
      i.dump();
  }

} // namespace Network
