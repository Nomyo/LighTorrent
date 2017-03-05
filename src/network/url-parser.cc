#include "url-parser.hh"

namespace UrlParser
{
  UrlParser::UrlParser(std::string url)
  {
    std::string protocol;
    std::string host;
    std::string port;
    std::string body;

    // Parsing the protocol indicator first
    int counter = 0;
    while (url[counter] != ':')
      protocol += url[counter++];

    counter += 3; // we jump the '://'

    while (url[counter] != ':')
      host += url[counter++];

    counter += 1; // we jump the ':'

    while (url[counter] != '/')
      port += url[counter++];

    body = url.substr(counter, url.length() - counter);

    if (protocol == "http")
      protocol_ = urlProtocol::Http;
    else
      protocol_ = urlProtocol::Udp;

    host_ = host;
    port_ = atoi(port.c_str());
    body_ = body;
  }

  urlProtocol UrlParser::getProtocol() const
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
}
