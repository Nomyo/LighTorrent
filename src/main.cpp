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
  //std::string filename("tests/secretFamilyRecipes.torrent"); // http torrent
  std::string filename("tests/l.torrent"); // udp torrent

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

  std::cout << std::endl;

  TrackerConnector::TrackerConnector tc(&torrent);
  tc.announce(urlGenerated);
  //if (tc.sendRequest(urlGenerated))
    //std::cout << "result_body: " << tc.getResult() << "\n" << std::endl;

  //std::string result = tc.getResult();

  //auto result_node = getType<BType_ptr, BDico>(driver.bDecode(result));
  //std::cout << result_node << std::endl;

  //std::string peers = getDecode<BType_ptr, BString, std::string>(result_node.get("peers"));
  //const unsigned char *str = (const unsigned char *) peers.c_str();

  //std::cout << "peers size : " << peers.size() / 6 << std::endl;

  //int j = 0;
  //int port = 0;

  //std::cout << peers << std::endl;

  //for (int i = 0; i < peers.size(); ++i, ++j)
  //{
  //  unsigned int b = str[i];
  //  if (j == 4)
  //  {
  //    std::cout << ":";
  //    port = 256 * str[i];
  //  }
  //  else if (j == 5)
  //  {
  //    port += str[i];
  //    std::cout << port << "\n";
  //    port = 0;
  //    j = -1;
  //  }
  //  else
  //  {
  //    if (i % 6)
  //      std::cout << ".";
  //    std::cout << b;
  //  }
  //}

  return 0;
}
