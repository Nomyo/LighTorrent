#include <string.h>
#include "tracker-connector.hh"

namespace TrackerConnector
{
  TrackerConnector::TrackerConnector(std::string hostName)
  {
    hostName_ = hostName;
    opened_ = false;
  }

  int TrackerConnector::connect()
  {
    if (opened_)
      return -1;

    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
      return -1;

    opened_ = true;

    server_ = gethostbyname(hostName_.c_str());
    if (server_ == nullptr)
      return -2;

    bzero((char *) &servAddr_, sizeof(servAddr_));
    servAddr_.sin_family = AF_INET;
    bcopy((char *)server_->h_addr,
          (char *)&servAddr_.sin_addr.s_addr,
          server_->h_length);
    servAddr_.sin_port = htons(80);

    std::string request = REQUEST_PREFIX + "http://www.google.fr/" + REQUEST_SUFFIX;

    if (::connect(fd_, (struct sockaddr*)&servAddr_, sizeof(servAddr_)) < 0)
      std::cout << "Error while connecting to the host\n";

    if (write(fd_, request.c_str(), strlen(request.c_str())) < 0)
      std::cout << "Error while writing to socket\n";

    char buffer[6000];
    int stop = 0;
    int recvN = recv(fd_, &buffer, 6000, 0);
    while (recvN > 0 || stop < 15000)
    {
      if (recvN > 0)
      {
        std::cout << "\nrecvN : " << recvN << "\n";
        std::cout << buffer << std::endl;
      }
      bzero((char*)buffer, 6000);
      recvN = recv(fd_, &buffer, 6000, 0);
      stop++;
    }

    std::cout << "\nFinal recvN: " << recvN << "\nErrno: " << errno << "\n";

    return fd_;
  }
}
