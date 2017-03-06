#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "url-parser.hh"
#include "tracker-info.hh"

#define READ_BUF_SIZE 6000

namespace NetworkDriver
{
  class HttpDriver
  {
    using UrlParser = UrlParser::UrlParser;
    using TrackerInfo = TrackerInfo::TrackerInfo;

    public:
      // Ctor & Dtor
      HttpDriver();
      ~HttpDriver();

      int sendRequest(const UrlParser& urlParser, const TrackerInfo& trackerInfo);
      std::string readResult();
    private:
      int fd_;
  };
}
