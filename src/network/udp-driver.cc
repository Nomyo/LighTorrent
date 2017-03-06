#include "udp-driver.hh"

namespace NetworkDriver
{
  UdpDriver::UdpDriver()
  {}

  UdpDriver::~UdpDriver()
  {}

  int UdpDriver::sendRequest(const TrackerInfo& trackerInfo)
  {
    std::cout << "requesting udp tracker..." << std::endl;

    auto request = createRequestAnnounce(1500);
    struct connectionResponse response;

    bool receivedPackets = false;
    int nbAttempt = 0;

    while (!receivedPackets && nbAttempt > -1)
    {
      int fd = socket(AF_INET, SOCK_DGRAM, 0);
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 100000;
      if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv)) < 0)
        std::cerr << "Could not set timeout to udp socket..." << std::endl;

      nbAttempt++;
      int nbSend = sendto(fd, &request, sizeof (request), 0,
                          (struct sockaddr *)&trackerInfo.getServerAddress(),
                          sizeof (trackerInfo.getServerAddress()));
      std::cout << "sent " << nbSend << " bytes... (attempt " << nbAttempt << "...)" << std::endl;

      socklen_t res = 4;
      int nbRecv = recvfrom(fd, &response, sizeof (response), 0,
                            (struct sockaddr *)&trackerInfo.getServerAddress(),
                            &res);

      if (nbRecv != -1)
        receivedPackets = true;
      close(fd);
    }
    if (receivedPackets)
    {
      std::cout << "received response!" << std::endl;
      std::cout << "action: " << response.action << "\n";
      std::cout << "transacitonId: " << response.transactionId << "\n";
      std::cout << "connectionId: " << response.connectionId << "\n";
    }

    return -1;
  }

  struct connectionRequest createRequestAnnounce(int transactionId)
  {
    struct connectionRequest cr;
    cr.connectionId = __builtin_bswap64(0x41727101980); // magic number
    cr.action = 0; // announce
    cr.transactionId = transactionId;

    return cr;
  }
}
