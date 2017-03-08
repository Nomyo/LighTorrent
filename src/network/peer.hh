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

  public:
    // Ctor & Dtor
    Peer(const std::string& ip, in_port_t port);
    ~Peer() = default;

    std::string getIp();
    in_port_t getPort();

    void dump();

  private:
    std::string ip_;
    in_port_t port_;

  };

} // namespace Network
