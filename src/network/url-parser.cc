#include "url-parser.hh"

namespace Network
{
  UrlParser::UrlParser()
  {}

  UrlParser::UrlParser(const std::string& url)
  {
    parseUrl(url);
  }

  void UrlParser::parseUrl(const std::string url)
  {
    baseUrl_ = url;

    std::string protocol;
    std::string host;
    std::string port;
    std::string body;

    // Parsing the protocol indicator first
    int counter = 0;
    while (url[counter] != ':')
      protocol += url[counter++];

    counter += 3; // we jump the '://'

    while (url[counter] != ':' && url[counter] != '/')
      host += url[counter++];

    if (protocol == "udp")
    {
      counter += 1; // we jump the ':'

      while (url[counter] != '/')
        port += url[counter++];
    }
    else
      port = "80";

    body = url.substr(counter, url.length() - counter);

    if (protocol == "http")
      protocol_ = UrlProtocol::Http;
    else
      protocol_ = UrlProtocol::Udp;

    host_ = host;
    port_ = atoi(port.c_str());
    body_ = body;
  }

  UrlProtocol UrlParser::getProtocol() const
  {
    return protocol_;
  }

  std::string UrlParser::getHost() const
  {
    return host_;
  }

  int UrlParser::getPort() const
  {
    return port_;
  }

  std::string UrlParser::getBody() const
  {
    return body_;
  }

  std::string UrlParser::getBaseUrl() const
  {
    return baseUrl_;
  }

  void UrlParser::dump() const
  {
    std::cout << "url : " << baseUrl_ << std::endl;
    std::cout << "protocol : " << protocol_ << " (0 is Http, 1 is Udp)" << std::endl;
    std::cout << "host : " << host_ << std::endl;
    std::cout << "port : " << port_ << std::endl;
    std::cout << "body : " << body_ << std::endl;
  }
}
