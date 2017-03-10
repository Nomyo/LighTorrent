#include "message-builder.hh"

namespace Core
{
  MessageBuilder::MessageBuilder()
  { }

  Message MessageBuilder::buildHandshake(const Torrent& torrent)
  {

    auto *hshake = (struct handshake *)calloc(1, sizeof (struct handshake));

    std::string info_hash = torrent.getInfoHash();
    std::string peer_id = torrent.getPeerId();
    std::string protocol = "BitTorrent protocol";

    hshake->pstrlen = 19;

    memcpy(hshake->pstr, protocol.c_str(), 19);
    memcpy(hshake->info_hash, info_hash.c_str(), 20);
    memcpy(hshake->peer_id, peer_id.c_str(), 20);

    return Message((char *)hshake, 68);
  }

  Message MessageBuilder::buildKeepAlive()
  {
    return Message((char *)calloc(1, 4), 4);
  }

  Message MessageBuilder::buildChoke()
  {
    char *buffer = (char *)calloc(1, 5);
    buffer[3] = 1;
    return Message(buffer ,5);
  }

  Message MessageBuilder::buildUnChoke()
  {
    char *buffer = (char *)calloc(1, 5);
    buffer[3] = 1;
    buffer[4] = 1;
    return Message(buffer ,5);
  }

  Message MessageBuilder::buildInterested()
  {
    char *buffer = (char *)calloc(1, 5);
    buffer[3] = 1;
    buffer[4] = 2;
    return Message(buffer ,5);
  }

  Message MessageBuilder::buildNotInterested()
  {
    char *buffer = (char *)calloc(1, 5);
    buffer[3] = 1;
    buffer[4] = 3;
    return Message(buffer ,5);
  }

} // namespace Core
