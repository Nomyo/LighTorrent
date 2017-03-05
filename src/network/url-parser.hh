#include <string>
#include <iostream>

namespace UrlParser
{
  enum UrlProtocol
  {
    Http,
    Udp
  };

  class UrlParser
  {
    public:
      UrlParser(std::string url);
      std::string getHost() const;
      std::string getProtocol() const;
      std::string getPort() const;

    private:
  };
}
