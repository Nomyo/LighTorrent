#pragma once

#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include "message.hh"
#include "torrent.hh"

namespace Core
{
  class MessageBuilder
  {

    struct handshake
    {
      uint8_t pstrlen;
      uint8_t pstr[19];
      uint8_t reserved[8];
      uint8_t info_hash[20];
      uint8_t peer_id[20];
    };

  public:
    // Ctor & Dtor
    MessageBuilder();
    ~MessageBuilder() = default;

    // Action
    Message buildHandshake(const Torrent& torrent);
    Message buildKeepAlive();
    Message buildChoke();
    Message buildUnChoke();
    Message buildInterested();
    Message buildNotInterested();

  private:
  };

} // namespace Core
