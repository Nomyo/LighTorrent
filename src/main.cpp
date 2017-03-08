#include <iostream>

#include "bencode/bencode-driver.hh"
#include "bencode/bencode-utils.hh"
#include "bencode/fwd.hh"

#include "core/url-utils.hh"

#include "network/tracker-connector.hh"
#include "network/torrent.hh"
#include "network/url-parser.hh"

#include <stdio.h>

using namespace BEncode;

int main(void)
{
  //std::string filename("tests/let-it-be.torrent")a; // upd torrent
  std::string filename("tests/secretFamilyRecipes.torrent"); // http torrent

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
  tc.announce(urlGenerated);

  // std::string result = tc.getResult();

  // auto result_node = getType<BType_ptr, BDico>(driver.bDecode(result));

  // std::string peers = getDecode<BType_ptr, BString, std::string>(result_node.get("peers"));

  return 0;
}
