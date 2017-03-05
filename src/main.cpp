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
  // TrackerConnector::TrackerConnector tc;
  // if (tc.sendRequest("http://www.google.fr:80/"))
  //   std::cout << "result: \"" << tc.getResult() << "\"" << std::endl;

  std::string filename("tests/let-it-be.torrent");

  BEncodeDriver driver;
  auto node = driver.bDecodeFile(filename);

  if (!node)
    return 1;

  BDico dico = getType<BType_ptr, BDico>(node);

  Core::URLUtils url(dico);
  std::cout << "URL REQUEST : " << url.generateURL() << std::endl;;

  UrlParser::UrlParser(url.generateURL());

  return 0;
}
