#include <string.h>
#include "tracker-connector.hh"

#define READ_BUF_SIZE 6000

namespace TrackerConnector
{
  TrackerConnector::TrackerConnector()
  {}

  TrackerConnector::~TrackerConnector()
  {}

  int TrackerConnector::sendRequest(const std::string& url)
  {
    urlParser_.parseUrl(url);
    trackerInfo_.setTrackerInfo(urlParser_.getHost(), urlParser_.getPort());

    if (!createSocket())
      return -1;

    if (urlParser_.getProtocol() == UrlProtocol::Http)
    {
      HttpDriver httpDriver;
      httpDriver.sendRequest(urlParser_, trackerInfo_);
      result_ = httpDriver.getResult();
      return 1;
    }
    else
      return requestUdpTracker();
  }

  std::string TrackerConnector::getResult() const
  {
    return result_;
  }

  int TrackerConnector::createSocket()
  {
    if (opened_)
    {
      std::cerr << "cannot create socket: already active" << std::endl;
      return -1;
    }

    if (urlParser_.getProtocol() == UrlProtocol::Http)
      fd_ = socket(AF_INET, SOCK_STREAM, 0); // Http
    else
      fd_ = socket(AF_INET, SOCK_DGRAM, 0); // Udp

    if (fd_ == -1)
    {
      std::cerr << "could not create socket" << std::endl;
      return -1;
    }

    opened_ = true;

    return 1;
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

  int TrackerConnector::requestUdpTracker()
  {
    std::cout << "requesting udp tracker..." << std::endl;

    struct cir_t cir;
    cir.connectionId = __builtin_bswap64(0x41727101980);
    cir.action = 0;
    cir.transactionId = 1500;

    int nbSend = sendto(fd_, &cir, sizeof (cir), 0,
                        (struct sockaddr *)&trackerInfo_.getServerAddress(),
                        sizeof (trackerInfo_.getServerAddress()));
    std::cout << "sent " << nbSend << " bytes..." << std::endl;

    struct cire_t cire;
    socklen_t res;
    int nbRecv = recvfrom(fd_, &cire, sizeof (cire), 0,
                          (struct sockaddr *)&trackerInfo_.getServerAddress(),
                          &res);

    std::cout << "received " << nbRecv << "bytes!" << std::endl;
    std::cout << "action: " << cire.action << "\n";
    std::cout << "transacitonId: " << cire.transactionId << "\n";
    std::cout << "connectionId: " << cire.connectionId << "\n";

    return -1;
  }
}
