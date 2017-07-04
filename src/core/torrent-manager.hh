#pragma once

#include "network/peer.hh"
#include "network/tracker-connector.hh"
#include "network/url-parser.hh"

#include "bencode/bencode-driver.hh"
#include "bencode/bencode-utils.hh"
#include "bencode/fwd.hh"

#include "url-utils.hh"
#include "tracker-driver.hh"
#include "peer-driver.hh"
#include "torrent.hh"

using namespace BEncode;

namespace Core
{
  class TorrentManager
  {
  public:
    // Ctor & Dtor
    TorrentManager();
    TorrentManager(Torrent torrent);
    ~TorrentManager();

    void download(const std::string& filename);

  private:
    Torrent torrent_;
  };

} // namespace Network
