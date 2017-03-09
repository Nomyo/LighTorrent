#include "client.hh"

namespace Core
{

  Client::Client()
  { }

  Client::Client(Torrent torrent)
    : torrent_(torrent)
  { }

  Client::~Client()
  { }

  void Client::download(const std::string& filename)
  {
    BEncodeDriver driver;
    auto decodedFile = driver.bDecodeFile(filename);

    if (!decodedFile)
    {
      std::cerr << filename << " is not a valid torrent file" << std::endl;
      return;
    }

    torrent_ = (getType<BType_ptr, BDico>(decodedFile));

    Core::URLUtils url;
    std::string urlGenerated = url.generateURL(torrent_);
    Network::UrlParser up(urlGenerated);
    up.dump();
    std::cout << std::endl;

    TrackerDriver trackDriver(&torrent_);
    trackDriver.createConnectors();
    trackDriver.announces();

    Network::TrackerConnector tc(&torrent_);
    PeerDriver peerDriver(tc.announce(urlGenerated));
    peerDriver.startLeeching();
    peerDriver.dumpPeers();
    //peers_ = tc.announce(urlGenerated);
  }
} // namespace Network
