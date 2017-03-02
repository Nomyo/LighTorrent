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

    /*bzero((char *) &servAddr_, sizeof(servAddr_));
    servAddr_.sin_family = AF_INET;
    bcopy((char *)server_->h_addr,
          (char *)&servAddr_.sin_addr.s_addr,
          server_->h_length);
    servAddr_.sin_port = htons(80);*/

    return fd_;
  }
}