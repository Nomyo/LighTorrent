#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>

#include "../bencode/fwd.hh"
#include "../bencode/bencode-driver.hh"
#include "../bencode/bencode-utils.hh"

#include "url-parser.hh"
#include "tracker-info.hh"
#include "peer.hh"
#include "utils.hh"

#define READ_BUF_SIZE 6000

namespace Network
{
  class HttpDriver
  {
    public:
      // Ctor & Dtor
      HttpDriver();
      ~HttpDriver();

      std::vector<Peer> announce(const UrlParser& urlParser, const TrackerInfo& trackerInfo);
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
