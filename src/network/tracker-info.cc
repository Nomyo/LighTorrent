#include "tracker-info.hh"

namespace TrackerInfo
{
  TrackerInfo::TrackerInfo()
  {}

  TrackerInfo::TrackerInfo(const std::string& host, int port)
  {
    setTrackerInfo(host, port);
  }

  TrackerInfo::~TrackerInfo()
  {}

  void TrackerInfo::setTrackerInfo(const std::string& host, int port)
  {
    server_ = gethostbyname(host.c_str());

    if (server_ == nullptr)
      std::cerr << "Could not resolve host " << host << std::endl;

    bzero((char *)&serverAddress_, sizeof (serverAddress_));

    serverAddress_.sin_family = AF_INET;

    bcopy((char *)server_->h_addr,
          (char *)&serverAddress_.sin_addr.s_addr,
          server_->h_length);

    serverAddress_.sin_port = htons(port);
  }

  const struct sockaddr_in& TrackerInfo::getServerAddress() const
  {
    return serverAddress_;
  }

  const struct hostent *TrackerInfo::getServer() const
  {
    return server_;
  }

  bool TrackerInfo::isResolved() const
  {
    return (server_ != nullptr);
  }
}
