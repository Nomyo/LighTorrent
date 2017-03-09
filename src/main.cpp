#include "network/client.hh"

int main(void)
{
  Network::Client client;
  client.download("tests/l.torrent");
  //client.download("tests/secretFamilyRecipes.torrent");

  return 0;
}
