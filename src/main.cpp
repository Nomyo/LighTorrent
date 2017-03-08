#include <iostream>

#include "bencode/bencode-driver.hh"
#include "bencode/bencode-utils.hh"
#include "bencode/fwd.hh"

#include "core/url-utils.hh"

#include "network/tracker-connector.hh"
#include "network/torrent.hh"
#include "network/url-parser.hh"
#include "network/client.hh"

#include <stdio.h>

int main(void)
{

  Network::Client client;

  // Should be later add to download
  client.download("tests/secretFamilyRecipes.torrent");

  return 0;
}
