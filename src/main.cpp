#include <iostream>

#include "bencode/bencode-driver.hh"
#include "bencode/bencode-utils.hh"
#include "bencode/fwd.hh"

#include "core/url-utils.hh"

#include "network/tracker-connector.hh"
#include "network/torrent.hh"
#include "network/url-parser.hh"
#include "network/client.hh"

#include <stdio.h>

using namespace BEncode;

int main(void)
{
   std::string filename("tests/secretFamilyRecipes.torrent"); // http torrent
 // std::string filename("tests/let-it-be.torrent"); // udp torrent

  BEncodeDriver driver;
  auto node = driver.bDecodeFile(filename);
  if (!node)
    return 1;

  Network::Torrent torrent(getType<BType_ptr, BDico>(node));
  torrent.dump();

  Core::URLUtils url;
  std::string urlGenerated = url.generateURL(torrent);
  UrlParser::UrlParser up(urlGenerated);
  up.dump();
  std::cout << std::endl;

  TrackerConnector::TrackerConnector tc(&torrent);
  std::string result = tc.announce(urlGenerated);
  auto result_node = getType<BType_ptr, BDico>(driver.bDecode(result));
  std::string peersBinary = getDecode<BType_ptr, BString, std::string>(result_node.get("peers"));

  Network::Client client(torrent);
  client.getPeersFromBinary(peersBinary);
  client.dumpPeers();
  client.connectToPeers();

  return 0;
}
