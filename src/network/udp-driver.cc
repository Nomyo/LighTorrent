#include "udp-driver.hh"

#define TR_ID 1500
#define NB_RETRY 15
#define TIMEOUT_SEC 2

namespace NetworkDriver
{
  using Peer = Network::Peer;

  UdpDriver::UdpDriver(Torrent *t, const TrackerInfo& trackerInfo)
  {
    torrent_ = t;
    trackerInfo_ = trackerInfo;
  }

  UdpDriver::~UdpDriver()
  {}

  uint64_t UdpDriver::tryConnect()
  {
    auto request = createRequestAnnounce(TR_ID);
    struct connectionResponse response;

    bool receivedPackets = false;
    int nbAttempt = 0;

    while (!receivedPackets && nbAttempt < NB_RETRY)
    {
      fd_ = socket(AF_INET, SOCK_DGRAM, 0);
      setSocketTimeout(fd_);

      nbAttempt++;
      int nbSend = sendto(fd_, &request, sizeof (request), 0,
          (struct sockaddr *)&trackerInfo_.getServerAddress(),
          sizeof (trackerInfo_.getServerAddress()));
      if (nbSend == -1)
      {
        std::cerr << "Could not send connect request to tracker..." << std::endl;
        return 0;
      }
      std::cout << "connecting... (attempt " << nbAttempt << "...)" << std::endl;

      socklen_t res = 0;
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

    return response.connectionId;
  }

  std::vector<Peer> UdpDriver::tryAnnounce(uint64_t connectionId)
  {
    std::vector<Peer> peers;
    struct announceRequest ar = createAnnounceRequest(torrent_, connectionId);
    struct announceResponse response;

    bool receivedPackets = false;
    int nbAnnounce = 1;

    while (!receivedPackets && nbAnnounce < NB_RETRY)
    {

      int nbSend = sendto(fd_, &ar, sizeof (ar), 0,
          (struct sockaddr *)&trackerInfo_.getServerAddress(),
          sizeof (trackerInfo_.getServerAddress()));
      if (nbSend == -1)
      {
        std::cerr << "Could not send announce message to tracker..." << std::endl;
        return peers;
      }
      std::cout << "announcing... (attempt " << nbAnnounce << "...)" << std::endl;

      socklen_t res = 0;
      int nbRecv = recvfrom(fd_, &response, sizeof (response), 0,
          (struct sockaddr *)&trackerInfo_.getServerAddress(),
          &res);

      if (nbRecv != -1)
      {
        receivedPackets = true;
        std::cout << "received " << nbRecv << " bytes" << std::endl;
        std::cout << "received response" << std::endl
          << "action: " << __builtin_bswap32(response.action) << std::endl
          << "tId: " << __builtin_bswap32(response.transactionId) << std::endl
          << "interval: " << __builtin_bswap32(response.interval) << std::endl
          << "leechers: " << __builtin_bswap32(response.leechers) << std::endl
          << "seeders: " << __builtin_bswap32(response.seeders)
          << std::endl;

        peers = buildPeers(response.peer_infos, (nbRecv - 20) / 6);
        for (auto peer : peers)
          peer.dump();
      }
      else
      {
        close(fd_);
        fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        setSocketTimeout(fd_);
      }
      nbAnnounce++;
    }
    close(fd_);
    return peers;
  }

  std::vector<Peer> UdpDriver::buildPeers(const uint8_t *peer_info, int nb_peers)
  {
    std::vector<Peer> peers;
    for (int i = 0; i < nb_peers; i++)
    {
      std::string ip;
      for (int j = 0; j < 4; j++)
      {
        unsigned b = peer_info[i * 6 + j];
        ip += std::to_string(b);
        if (j != 3)
          ip += ".";
      }
      uint16_t port = 0;
      port += peer_info[i * 6 + 4] * 256;
      port += peer_info[i * 6 + 5];
      peers.push_back(Peer(ip, port));
    }
    return peers;
  }

  std::vector<Peer> UdpDriver::announce()
  {
    uint64_t connectionId = tryConnect();
    if (connectionId != 0)
      return tryAnnounce(connectionId);
    else
      return std::vector<Peer>();
  }

  struct connectionRequest createRequestAnnounce(int transactionId)
  {
    struct connectionRequest cr;
    cr.connectionId = __builtin_bswap64(0x41727101980); // magic number
    cr.action = 0; // announce
    cr.transactionId = transactionId;

    return cr;
  }

  struct announceRequest createAnnounceRequest(Network::Torrent *t, uint64_t connectionId)
  {
    std::string test = t->getInfoHash();
    std::string peer_id = t->getPeerId();

    struct announceRequest ar;
    ar.connectionId = connectionId;
    ar.action = __builtin_bswap32(1);
    ar.transactionId = __builtin_bswap32(TR_ID);
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

  void setSocketTimeout(int fd)
  {
    struct timeval tv;
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = 0;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv)) < 0)
      std::cerr << "Could not set timeout to udp socket..." << std::endl;
  }
}
