#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>

#include "bencode/fwd.hh"
#include "bencode/bencode-driver.hh"
#include "bencode/bencode-utils.hh"

#include "url-parser.hh"
#include "tracker.hh"
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

      // Returns a list of peers for a given tracker
      std::vector<Peer> announce(const UrlParser& urlParser, const Tracker& tracker);

    private:
      // Sends a request to a given tracker
      int sendRequest(const UrlParser& urlParser, const Tracker& tracker);
      // Reads the result of the HTTP request
      void formatResult(std::string& result);
      // Builds the result from the return message
      void buildResult();

      int fd_;
      std::string resultHeader_;
      std::string resultBody_;
  };
}
