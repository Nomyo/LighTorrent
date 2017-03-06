#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sstream>

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
      void formatResult(std::string& result);
      void deleteChunkInfo(std::string& s);

      int fd_;
      std::string resultHeader_;
      std::string resultBody_;
  };
}
