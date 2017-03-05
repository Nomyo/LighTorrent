#include <string>
#include <iostream>

namespace UrlParser
{
  enum urlProtocol
  {
    Http,
    Udp
  };

  class UrlParser
  {
    public:
      UrlParser(std::string url);
      std::string getHost() const;
      urlProtocol getProtocol() const;
      std::string getPort() const;

    private:
      urlProtocol protocol_;
      std::string host_;
      int port_;
      std::string body_;
  };
}
