#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "url-parser.hh"

namespace NetworkDriver
{
  class HttpDriver
  {
    using UrlParser = UrlParser::UrlParser;

    public:
      // Ctor & Dtor
      HttpDriver();
      ~HttpDriver();

      int sendRequest(UrlParser& urlParser);
    private:
      int fd_;
  };
}
