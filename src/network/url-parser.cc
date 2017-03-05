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

    std::cout << "protocol: " << protocol << std::endl;
    std::cout << "host: " << host << std::endl;
    std::cout << "port: " << port << std::endl;

    //int counterStart = 0;
    //bool hasHttp = false;
    //if (url.substr(0, 7) == "http://")
    //{
    //  counterStart = 7;
    //  hasHttp = true;
    //}
    //while (url[counterStart] != '/' && url[counterStart] != ':')
    //  counterStart++;

    //host_ = url.substr(hasHttp ? 7 : 0, counterStart - (hasHttp ? 7 : 0));

    //if (url[counterStart] == ':')
    //{
    //  int left = ++counterStart;
    //  while (url[counterStart] != '/')
    //    counterStart++;
    //  std::string num = url.substr(left, counterStart - left);
    //  port_ = atoi(num.c_str());
    //}
    //else
    //  port_ = 80;

    //request_ = url.substr(counterStart, url.length() - counterStart);

    //std::cout << "host: " << host_ << std::endl;
    //std::cout << "port: " << port_ << std::endl;
    //std::cout << "request: " << request_ << std::endl;
  }
}
