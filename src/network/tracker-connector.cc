#include <string.h>
#include "tracker-connector.hh"

namespace TrackerConnector
{
  TrackerConnector::TrackerConnector()
  {
    opened_ = false;
  }

  TrackerConnector::~TrackerConnector()
  {
    if (opened_)
      close(fd_);
  }

  int TrackerConnector::connect()
  {
    createSocket();

    resolveHost("www.laginelle-france.com");

    char buffer[6000];
    int stop = 0;
    int recvN = recv(fd_, &buffer, 6000, 0);
    while (recvN > 0 || stop < 15000)
    {
      if (recvN > 0)
      {
        std::cout << "\nrecvN : " << recvN << "\n";
        std::cout << buffer << std::endl;
        if (buffer[recvN - 1] == '\n' && buffer[recvN - 2] == '\r')
          break;
      }
      bzero((char*)buffer, 6000);
      recvN = recv(fd_, &buffer, 6000, 0);
      stop++;
    }

    std::cout << "\nFinal recvN: " << recvN << "\nErrno: " << errno << "\n";

    return fd_;
  }

  int TrackerConnector::createSocket()
  {
    if (opened_)
      return -1;

    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
      return -1;

    opened_ = true;

    return 1;
  }

  int TrackerConnector::resolveHost(std::string host)
  {
    server_ = gethostbyname(host.c_str());

    if (server_ == nullptr)
      return -1;

    bzero((char *) &servAddr_, sizeof(servAddr_));

    servAddr_.sin_family = AF_INET;

    bcopy((char *)server_->h_addr,
          (char *)&servAddr_.sin_addr.s_addr,
          server_->h_length);

    servAddr_.sin_port = htons(80); // HTTP PROTOCOL

    host_ = host;

    return 1;
  }

  int TrackerConnector::sendRequest()
  {
    std::string request = REQUEST_PREFIX + "/" + REQUEST_SUFFIX + "Host: "
      + host_ + "\r\n\r\n";

    std::cout << "Sending request \"" << request << "\"\n";

    if (::connect(fd_, (struct sockaddr*)&servAddr_, sizeof(servAddr_)) < 0)
      return -1;

    if (write(fd_, request.c_str(), strlen(request.c_str())) < 0)
      return -1;

    return 1;
  }
}
