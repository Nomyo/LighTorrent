#include <iostream>

#include "network/tracker-connector.hh"
#include "bencode/bencode-driver.hh"

int main(void)
{
  TrackerConnector::TrackerConnector tc;
  if (tc.sendRequest("http://www.google.fr:80/"))
    std::cout << "result: \"" << tc.getResult() << "\"" << std::endl;

  std::string filename("tests/secretFamilyRecipes.torrent");

  BEncode::BEncodeDriver driver;

  auto node = driver.bDecodeFile(filename);
  std::cout << node << std::endl;
  return 0;
}
