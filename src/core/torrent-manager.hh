#pragma once

#include "network/peer.hh"
#include "network/tracker/announcer.hh"
#include "network/url-parser.hh"

#include "bencode/bencode-driver.hh"
#include "bencode/bencode-utils.hh"
#include "bencode/fwd.hh"

#include "core/url-utils.hh"
#include "core/tracker-driver.hh"
#include "core/peer-driver.hh"
#include "core/torrent.hh"

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
