#include "torrent-manager.hh"

namespace Core
{

  TorrentManager::TorrentManager()
  { }

  TorrentManager::TorrentManager(Torrent torrent)
    : torrent_(torrent)
  { }

  TorrentManager::~TorrentManager()
  { }

  void TorrentManager::download(const std::string& filename)
  {
    BEncodeDriver driver;
    auto decodedFile = driver.bDecodeFile(filename);

    if (!decodedFile)
    {
      std::cerr << filename << " is not a valid torrent file" << std::endl;
      return;
    }

    torrent_ = Torrent(getType<BType_ptr, BDico>(decodedFile), filename);

    Core::URLUtils url;
    std::string urlGenerated = url.generateURL(torrent_);
    Network::Announcer tc(&torrent_);
    PeerDriver peerDriver(tc.announce(urlGenerated), &torrent_);

  }
} // namespace Network
