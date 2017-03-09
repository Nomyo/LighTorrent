#pragma once

#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

namespace Network
{
  class Peer
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
    Peer(const std::string& ip, in_port_t port);
    ~Peer() = default;


    // Action
    void tryHandshake();
    void onReceive();

    void dump() const;

    // Getter
    std::string getIp() const;
    in_port_t getPort() const;
    int getFd() const;

  private:
    Torrent torrent_;
    std::string ip_;
    in_port_t port_;
    int fd_;
    bool handshakeDone_ = false;
  };

} // namespace Network
