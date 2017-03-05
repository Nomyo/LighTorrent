#include <iostream>

#include "network/tracker-connector.hh"
#include "bencode/bencode-driver.hh"
#include "bencode/bencode-utils.hh"
#include "bencode/fwd.hh"
#include "core/url-utils.hh"
#include "network/url-parser.hh"

#include <stdio.h>

using namespace BEncode;

int main(void)
{
  std::string filename("tests/AliceInWonderlands.torrent"); // http torrent
  //std::string filename("tests/let-it-be.torrent"); // udp torrent


  BEncodeDriver driver;
  auto node = driver.bDecodeFile(filename);

  if (!node)
    return 1;

  BDico dico = getType<BType_ptr, BDico>(node);

  Core::URLUtils url(dico);
  std::cout << "URL REQUEST : " << url.generateURL() << std::endl;;

  UrlParser::UrlParser up(url.generateURL());
  up.dump();

  TrackerConnector::TrackerConnector tc;
  if (tc.sendRequest(url.generateURL()))
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
