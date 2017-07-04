#include "tracker.hh"

namespace Network
{
  Tracker::Tracker()
  {}

  Tracker::Tracker(const std::string& host, int port)
  {
    setTracker(host, port);
  }

  Tracker::~Tracker()
  {}

  void Tracker::setTracker(const std::string& host, int port)
  {
    server_ = gethostbyname(host.c_str());

    if (server_ == nullptr)
      std::cerr << "Could not resolve host " << host << std::endl;
    else
    {

      bzero((char *)&serverAddress_, sizeof(serverAddress_));

      serverAddress_.sin_family = AF_INET;

      bcopy((char *)server_->h_addr,
          (char *)&serverAddress_.sin_addr.s_addr,
          server_->h_length);

      serverAddress_.sin_port = htons(port);
    }
  }

  const struct sockaddr_in& Tracker::getServerAddress() const
  {
    return serverAddress_;
  }

  const struct hostent *Tracker::getServer() const
  {
    return server_;
  }

  bool Tracker::isResolved() const
  {
    return (server_ != nullptr);
  }
}
