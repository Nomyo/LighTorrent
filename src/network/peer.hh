#pragma once

#include "../core/torrent.hh"
#include "../core/message-builder.hh"
#include "../core/file-manager.hh"

#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <bitset>

#define READ_BUF_SIZE 6000

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
    void toSend(const Core::Message& m);
    void onReceive();
    void parseMessage(std::string msg);

    void tryHandshake();
    void onReceiveHandshake();
    void dump() const;
    bool operator==(const Peer& p);

    // Getter
    std::string getIp() const;
    in_port_t getPort() const;
    int getFd() const;
    long unsigned getLastUpdate() const;
    bool handshakeDone() const;

    //Setter
    void setTorrent(Core::Torrent* torrent);
    void setFileManager(Core::FileManager* fileManager);
    void setFd(int fd);
    void setLastUpdate(long unsigned update);

  private:
    std::vector<bool> pieces_;
    Core::Torrent* torrent_;
    Core::FileManager* fileManager_;
    std::string ip_;
    in_port_t port_;
    int fd_;
    long unsigned lastUpdate_;
    bool handshakeDone_ = false;
  };

} // namespace Network
