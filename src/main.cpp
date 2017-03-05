#include <iostream>

#include "network/tracker-connector.hh"
#include "bencode/bencode-driver.hh"
#include "bencode/bencode-utils.hh"
#include "bencode/fwd.hh"

#include "core/url-utils.hh"
#include "core/torrent.hh"

#include "network/url-parser.hh"

#include <stdio.h>

using namespace BEncode;

int main(void)
{
  std::string filename("tests/secretFamilyRecipes.torrent"); // http torrent
  //std::string filename("tests/let-it-be.torrent"); // udp torrent

  BEncodeDriver driver;
  auto node = driver.bDecodeFile(filename);
  if (!node)
    return 1;

  std::cout << "Torrent decoded :\n" << node << std::endl;

  Core::Torrent torrent(getType<BType_ptr, BDico>(node));
  Core::URLUtils url;
  std::string urlGenerated = url.generateURL(torrent);
  std::cout << "URL REQUEST : " << urlGenerated << std::endl;;

  UrlParser::UrlParser up(urlGenerated);
  up.dump();

  TrackerConnector::TrackerConnector tc;
  if (tc.sendRequest(urlGenerated))
  {
    //std::cout << "result: " << tc.getResult() << std::endl;
    std::cout << "result_body: " << tc.getResultBody() << std::endl;
  }

  std::cout << std::endl;

  std::string result = tc.getResultBody();
  auto result_node = driver.bDecode(result);

  std::cout << result_node << std::endl;


  return 0;
}
