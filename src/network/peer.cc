#include "peer.hh"

namespace Network
{

  Peer::Peer(const std::string& ip, in_port_t port)
    : ip_(ip)
    , port_(port)
    , fd_(0)
  { }


  void Peer::toSend(const Core::Message& m)
  {
    write(fd_, m.getBuffer(), m.getLength());
  }

  void Peer::onReceive()
  {
    if (!handshakeDone_)
      onReceiveHandshake();
    else
    {
      std::string res = "";
      char buffer[READ_BUF_SIZE];
      int recvN = recv(fd_, buffer, READ_BUF_SIZE - 1, 0);
      if (recvN > 0)
      {
        for (int i = 0; i < recvN; i++)
          res += buffer[i];
        bzero(buffer, READ_BUF_SIZE);
        totalReceived_ = recvN;
        if (recvN >= READ_BUF_SIZE - 1)
        {
          std::cout << "SHOULDN'T BE THERE FOR NOW" << std::endl;
          exit(27);
        }
      }
      parseMessage(res);

      if (expectReceived_ == totalReceived_)
      {
        // Do action
      }

    }
  }

  void Peer::parseMessage(std::string msg)
  {

    expectReceived_ = computeLength(msg.substr(0, 4));

    //std::cout << "length " << expectReceived_ << std::endl;

    for (unsigned i = 0; i < msg.size(); ++i)
    {
      uint8_t j = msg[i];
      //printf("%x\n", j);
    }

    unsigned int i = 0;
    memcpy(&i, msg.c_str(), sizeof(i));

    /*std::cout << "bitset : " << htons(i) << std::endl;
    if (msg.length() < 5)
      std::cout << "Message: keep alive" << std::endl;
    else if (msg.length() == 5)
      std::cout << "No payload message" << std::endl;
    else
    {
      if (msg[4] == 4)
        std::cout << "HAVE messages" << std::endl;
      else if (msg[4] == 5)
        std::cout << "BITFIELDS messages" << std::endl;
      else if (msg[4] == 6)
        std::cout << "REQUEST messages" << std::endl;
    }
    */
  }

  void Peer::tryHandshake()
  {
    Core::MessageBuilder mBuilder;
    toSend(mBuilder.buildHandshake(*torrent_));
  }

  void Peer::onReceiveHandshake()
  {
    struct handshake *ret;
    char buffer[sizeof (struct handshake)];
    int recvN = recv(fd_, buffer, sizeof (struct handshake), 0);
    if (recvN > 0)
    {
      ret = (struct handshake *)buffer;
      if (memcmp(ret->info_hash, torrent_->getInfoHash().c_str(), 20) == 0)
        handshakeDone_ = true;
    }
  }

  void Peer::dump() const
  {
    Core::ColorModifier clgreen(Core::ColorCode::FG_LIGHT_BLUE);
    Core::ColorModifier clblue(Core::ColorCode::FG_BLUE);
    Core::ColorModifier cdef(Core::ColorCode::FG_DEFAULT);

    std::cout << clblue << ip_  << " " << cdef << " "
              << clgreen << port_ << cdef << std::endl;
  }

  bool Peer::operator==(const Peer& p)
  {
    return (port_ == p.port_) && (ip_ == p.ip_);
  }


  long long int Peer::computeLength(std::string lengthPrefix)
  {
    uint8_t l_1 = lengthPrefix[0];
    uint8_t l_2 = lengthPrefix[1];
    uint8_t l_3 = lengthPrefix[2];
    uint8_t l_4 = lengthPrefix[3];

    long long int tmp =  l_1 * 256 * 256 * 256;
    tmp += l_2 * 256 * 256;
    tmp += l_3 * 256;
    tmp += l_4;

    return tmp;
  }


  // Getter
  std::string Peer::getIp() const
  {
    return ip_;
  }

  in_port_t Peer::getPort() const
  {
    return port_;
  }

  int Peer::getFd() const
  {
    return fd_;
  }

  bool Peer::handshakeDone() const
  {
    return handshakeDone_;
  }

  void Peer::setTorrent(Core::Torrent* torrent)
  {
    torrent_ = torrent;
  }

  void Peer::setFileManager(Core::FileManager* fileManager)
  {
    fileManager_ = fileManager;
  }

  void Peer::setFd(int fd)
  {
    fd_ = fd;
  }

} // namespace Network
