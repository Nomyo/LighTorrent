#include "udp-driver.hh"

namespace NetworkDriver
{
  UdpDriver::UdpDriver()
  {
    fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd_ == -1)
      std::cerr << "Could not create udp socket..." << std::endl;
  }

  UdpDriver::~UdpDriver()
  {
    if (fd_ != -1)
      close(fd_);
  }

struct cir_t {
    uint64_t connectionId;
    uint32_t action;
    uint32_t transactionId;
  };

  struct cire_t {
    uint32_t action;
    uint32_t transactionId;
    uint64_t connectionId;
  };

  int UdpDriver::sendRequest(const TrackerInfo& trackerInfo)
  {
    std::cout << "requesting udp tracker..." << std::endl;

    struct cir_t cir;
    cir.connectionId = __builtin_bswap64(0x41727101980);
    cir.action = 0;
    cir.transactionId = 1500;

    int nbSend = sendto(fd_, &cir, sizeof (cir), 0,
                        (struct sockaddr *)&trackerInfo.getServerAddress(),
                        sizeof (trackerInfo.getServerAddress()));
    std::cout << "sent " << nbSend << " bytes..." << std::endl;

    struct cire_t cire;
    socklen_t res;
    int nbRecv = recvfrom(fd_, &cire, sizeof (cire), 0,
                          (struct sockaddr *)&trackerInfo.getServerAddress(),
                          &res);

    std::cout << "received " << nbRecv << "bytes!" << std::endl;
    std::cout << "action: " << cire.action << "\n";
    std::cout << "transacitonId: " << cire.transactionId << "\n";
    std::cout << "connectionId: " << cire.connectionId << "\n";

    return -1;
  }
}
