#include "udp-driver.hh"

namespace NetworkDriver
{
  UdpDriver::UdpDriver(Torrent *t)
  {
    torrent_ = t;
  }

  UdpDriver::~UdpDriver()
  {}

  int UdpDriver::tryConnect()
  {
    std::cout << "requesting udp tracker..." << std::endl;

    auto request = createRequestAnnounce(1500);
    struct connectionResponse response;

    bool receivedPackets = false;
    int nbAttempt = 0;

    while (!receivedPackets && nbAttempt > -1)
    {
      fd_ = socket(AF_INET, SOCK_DGRAM, 0);
      struct timeval tv;
      tv.tv_sec = 2;
      tv.tv_usec = 0;
      if (setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv)) < 0)
        std::cerr << "Could not set timeout to udp socket..." << std::endl;

      nbAttempt++;
      int nbSend = sendto(fd_, &request, sizeof (request), 0,
          (struct sockaddr *)&trackerInfo_.getServerAddress(),
          sizeof (trackerInfo_.getServerAddress()));
      if (nbSend == -1)
      {
        std::cerr << "Could not send connect request to tracker..." << std::endl;
        return -1;
      }
      std::cout << "connecting... (attempt " << nbAttempt << "...)" << std::endl;

      socklen_t res = 4;
      int nbRecv = recvfrom(fd_, &response, sizeof (response), 0,
          (struct sockaddr *)&trackerInfo_.getServerAddress(),
          &res);

      if (nbRecv != -1)
        receivedPackets = true;
      else
        close(fd_);
    }

    if (receivedPackets)
    {
      std::cout << "action: " << response.action << "\n";
      std::cout << "transactionId: " << response.transactionId << "\n";
      std::cout << "connectionId: " << response.connectionId << "\n";
    }

    return tryAnnounce(response.connectionId, response.transactionId);
  }

  int UdpDriver::tryAnnounce(uint64_t connectionId, uint32_t transactionId)
  {
    struct announceRequest ar = createAnnounceRequest(torrent_, connectionId,
        transactionId);
    struct announceResponse response;

    bool received2 = false;
    int nbAnnounce = 1;

    while (!received2)
    {

      int nbSend = sendto(fd_, &ar, sizeof (ar), 0,
          (struct sockaddr *)&trackerInfo_.getServerAddress(),
          sizeof (trackerInfo_.getServerAddress()));
      if (nbSend == -1)
      {
        std::cerr << "Could not send announce message to tracker..." << std::endl;
        return -1;
      }
      std::cout << "announcing... (attempt " << nbAnnounce << "...)" << std::endl;

      socklen_t res = 4;
      int nbRecv = recvfrom(fd_, &response, sizeof (response), 0,
          (struct sockaddr *)&trackerInfo_.getServerAddress(),
          &res);

      if (nbRecv != -1)
      {
        std::vector<ipAddress> ips;
        received2 = true;
        std::cout << "received " << nbRecv << " bytes" << std::endl;
        std::cout << "received response" << std::endl
          << "action: " << __builtin_bswap32(response.action) << std::endl
          << "tId: " << __builtin_bswap32(response.transactionId) << std::endl
          << "interval: " << __builtin_bswap32(response.interval) << std::endl
          << "leechers: " << __builtin_bswap32(response.leechers) << std::endl
          << "seeders: " << __builtin_bswap32(response.seeders)
          << std::endl;
        for (int i = 0; i < (nbRecv - 20) / 6; i++)
        {
          struct ipAddress ip;
          for (int j = 0; j < 4; j++)
          {
            unsigned b = response.peer_infos[i * 6 + j];
            ip.ip += std::to_string(b);
            if (j != 3)
              ip.ip += ".";
          }
          unsigned int port = 256 * response.peer_infos[i * 6 + 4];
          port += response.peer_infos[i * 6 + 5];
          ip.port = port;
          ips.push_back(ip);
        }
        for (auto ip : ips)
          std::cout << ip.ip << ":" << ip.port << std::endl;
      }
      else
      {
        close(fd_);
        fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        struct timeval tv;
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        if (setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv)) < 0)
          std::cerr << "Could not set timeout to udp socket..." << std::endl;
      }
      nbAnnounce++;
    }
    return -1;
  }

  int UdpDriver::sendRequest(const TrackerInfo& trackerInfo)
  {
    trackerInfo_ = trackerInfo;

    return tryConnect();
  }

  struct connectionRequest createRequestAnnounce(int transactionId)
  {
    struct connectionRequest cr;
    cr.connectionId = __builtin_bswap64(0x41727101980); // magic number
    cr.action = 0; // announce
    cr.transactionId = transactionId;

    return cr;
  }

  struct announceRequest createAnnounceRequest(Network::Torrent *t, uint64_t connectionId,
      uint32_t transactionId)
  {
    std::string test = t->getInfoHash();
    std::string peer_id = t->getPeerId();

    struct announceRequest ar;
    ar.connectionId = connectionId;
    ar.action = __builtin_bswap32(1);
    ar.transactionId = __builtin_bswap32(transactionId);
    for (int i = 0; i < 20; i++)
      ar.info_hash[i] = test[i];
    for (int i = 0; i < 20; i++)
      ar.peer_id[i] = peer_id[i];
    ar.downloaded = t->getDownloaded();
    ar.left = t->getLeft();
    ar.uploaded = t->getUploaded();
    ar.event = 0;
    ar.ip = 0;
    ar.key = 0;
    ar.num_want = -1;
    ar.port = 0;

    return ar;
  }
}
