#include "network/client.hh"

int main(void)
{
  Network::Client client;
  client.download("tests/l.torrent");

  return 0;
}
