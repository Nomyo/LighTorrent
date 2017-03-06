#include "http-driver.hh"

namespace NetworkDriver
{
  HttpDriver::HttpDriver()
  {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ == -1)
      std::cerr << "Could not create socket..." << std::endl;
  }

  HttpDriver::~HttpDriver()
  {
    if (fd_ != -1)
      close(fd_);
  }

  int HttpDriver::sendRequest(UrlParser& urlParser)
  {
    return -1;
  }
}
