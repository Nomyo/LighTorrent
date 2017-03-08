#include "utils.hh"

namespace NetworkUtils
{
  std::vector<Network::Peer> buildPeers(const uint8_t *peers_info, int nb_peers)
  {
    std::vector<Network::Peer> peers;
    for (int i = 0; i < nb_peers; i++)
    {
      std::string ip;
      for (int j = 0; j < 4; j++)
      {
        unsigned b = peers_info[i * 6 + j];
        ip += std::to_string(b);
        if (j != 3)
          ip += ".";
      }
      uint16_t port = 0;
      port += peers_info[i * 6 + 4] * 256;
      port += peers_info[i * 6 + 5];
      peers.push_back(Network::Peer(ip, port));
    }
    return peers;

  }
}
