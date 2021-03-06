#pragma once

#include <string>
#include <iostream>

namespace Network
{
  enum UrlProtocol
  {
    Http,
    Udp
  };

  class UrlParser
  {
    public:
      //Ctor & Dtor
      UrlParser();
      UrlParser(const std::string& url);

      // Actions
      void parseUrl(const std::string url);

      // Accessors
      std::string getHost() const;
      UrlProtocol getProtocol() const;
      int getPort() const;
      std::string getBody() const;
      std::string getBaseUrl() const;

      // misc
      void dump() const;

    private:
      std::string baseUrl_;
      UrlProtocol protocol_;
      std::string host_;
      int port_;
      std::string body_;
  };
}
