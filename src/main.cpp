#include "core/client.hh"

int main(void)
{
  Core::Client client;
  client.download("tests/l.torrent");
  //client.download("tests/secretFamilyRecipes.torrent");

  return 0;
}
