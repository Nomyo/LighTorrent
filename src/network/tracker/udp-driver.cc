#include "udp-driver.hh"

#define TR_ID 1500
#define NB_RETRY 15
#define TIMEOUT_SEC 2

namespace Network
{
  using Peer = Network::Peer;

  UdpDriver::UdpDriver(Torrent *t, const Tracker& tracker)
  {
    torrent_ = t;
    tracker_ = tracker;
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
      int nbSend = sendto(fd_, &request, sizeof(request), 0,
          (struct sockaddr *)&tracker_.getServerAddress(),
          sizeof(tracker_.getServerAddress()));
      if (nbSend == -1)
      {
        std::cerr << "Could not send connect request to tracker..." << std::endl;
        return 0;
      }
      std::cout << "connecting... (attempt " << nbAttempt << "...)" << std::endl;

      socklen_t res = 0;
      int nbRecv = recvfrom(fd_, &response, sizeof(response), 0,
          (struct sockaddr *)&tracker_.getServerAddress(),
          &res);

      if (nbRecv != -1)
        receivedPackets = true;
      else
        close(fd_);
    }
    if (!receivedPackets)
      std::cout << "Failed to connect to tracker." << std::endl;

    return response.connectionId;
  }

  std::vector<Peer> UdpDriver::tryAnnounce(uint64_t connectionId)
  {
    std::vector<Peer> peers;
    struct announceRequest arequest = createAnnounceRequest(torrent_, connectionId);
    struct announceResponse aresponse;

    bool receivedPackets = false;
    int nbAnnounce = 1;

    while (!receivedPackets && nbAnnounce < NB_RETRY)
    {
      int nbSend = sendto(fd_, &arequest, sizeof(arequest), 0,
          (struct sockaddr *)&tracker_.getServerAddress(),
          sizeof(tracker_.getServerAddress()));
      if (nbSend == -1)
      {
        std::cerr << "Could not send announce message to tracker..." << std::endl;
        return peers;
      }
      std::cout << "announcing... (attempt " << nbAnnounce << "...)" << std::endl;

      socklen_t res = 0;
      int nbRecv = recvfrom(fd_, &aresponse, sizeof(aresponse), 0,
          (struct sockaddr *)&tracker_.getServerAddress(),
          &res);

      if (nbRecv != -1)
      {
        receivedPackets = true;
        peers = buildPeers(aresponse.peer_infos, (nbRecv - 20) / 6);
      }
      else
      {
        close(fd_);
        fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        setSocketTimeout(fd_);
      }
      nbAnnounce++;
    }
    if (!receivedPackets)
      std::cout << "No response from the tracker." << std::endl;

    close(fd_);
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

  struct announceRequest createAnnounceRequest(Core::Torrent *t, uint64_t connectionId)
  {
    std::string info_hash = t->getInfoHash();
    std::string peer_id = t->getPeerId();

    struct announceRequest ar;
    bzero(&ar, sizeof(ar));

    ar.connectionId = connectionId;
    ar.action = __builtin_bswap32(1);
    ar.transactionId = __builtin_bswap32(TR_ID);
    for (int i = 0; i < 20; i++)
      ar.info_hash[i] = info_hash[i];
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
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
      std::cerr << "Could not set timeout to udp socket..." << std::endl;
  }
}
