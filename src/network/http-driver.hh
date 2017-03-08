#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <list>

#include "../bencode/fwd.hh"
#include "../bencode/bencode-driver.hh"
#include "../bencode/bencode-utils.hh"
#include "url-parser.hh"
#include "tracker-info.hh"
#include "peer.hh"

#define READ_BUF_SIZE 6000

namespace NetworkDriver
{
  class HttpDriver
  {
    using UrlParser = UrlParser::UrlParser;
    using TrackerInfo = TrackerInfo::TrackerInfo;
    using Peer = Network::Peer;

    public:
      // Ctor & Dtor
      HttpDriver();
      ~HttpDriver();

      std::list<Peer> announce(const UrlParser& urlParser, const TrackerInfo& trackerInfo);
      int sendRequest(const UrlParser& urlParser, const TrackerInfo& trackerInfo);
      std::string getResult();

    private:
      void formatResult(std::string& result);
      void buildResult();

      int fd_;
      std::string resultHeader_;
      std::string resultBody_;
  };
}
