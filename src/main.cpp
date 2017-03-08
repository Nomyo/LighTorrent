#include "network/client.hh"

int main(void)
{
  std::string filename("tests/secretFamilyRecipes.torrent"); // http torrent
  //std::string filename("tests/let-it-be.torrent"); // udp torrent

  BEncodeDriver driver;
  auto node = driver.bDecodeFile(filename);
  if (!node)
    return 1;

  Network::Torrent torrent(getType<BType_ptr, BDico>(node));
  torrent.dump();

  Core::URLUtils url;
  std::string urlGenerated = url.generateURL(torrent);

  TrackerConnector::TrackerConnector tc(&torrent);
  std::list<Network::Peer> peers = tc.announce(urlGenerated);

  //Network::Client client(torrent);
  //client.getPeersFromBinary(peersBinary);
  //client.dumpPeers();

  return 0;
}
