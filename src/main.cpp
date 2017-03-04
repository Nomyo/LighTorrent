#include <iostream>

#include "network/tracker-connector.hh"
#include "bencode/bencode-driver.hh"

int main(void)
{
  TrackerConnector::TrackerConnector tc("www.google.com");

  std::cout << "connecting : " << tc.connect() << "\n";
  // std::cout << "lol\n";
  //
  std::cout << "\n\n-----\n\n";

  std::string filename("tests/secretFamilyRecipes.torrent");

  BEncode::BEncodeDriver driver;

  auto node = driver.bDecodeFile(filename);
  std::cout << node << std::endl;
  return 0;
}
