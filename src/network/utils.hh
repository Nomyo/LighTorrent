#pragma once

#include <vector>

#include "peer.hh"

namespace NetworkUtils
{
  std::vector<Network::Peer> buildPeers(const uint8_t *peers_info, int nb_peers);
}
