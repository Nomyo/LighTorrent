#include "network/client.hh"

int main(void)
{

  Network::Client client;

  // Should be later add to download
  client.download("tests/secretFamilyRecipes.torrent");

  return 0;
}
