#pragma once

#include <vector>

#include "peer.hh"

namespace Network
{
  std::vector<Network::Peer> buildPeers(const uint8_t *peers_info, int nb_peers);
}
