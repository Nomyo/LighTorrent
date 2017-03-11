#include "peer.hh"
#include "../core/color.hh"
#include <stdio.h>
#include <bitset>

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
      while (recvN > 0)
      {
	buffer[READ_BUF_SIZE - 1] = '\0';
	for (int i = 0; i < recvN; i++)
	  res += buffer[i];
	bzero(buffer, READ_BUF_SIZE);
	if (recvN < READ_BUF_SIZE - 1)
	  break;
	else
	  std::cout << "SHOULDN'T BE THERE FOR NOW" << std::endl;
      }
      parseMessage(res);
    }
  }

  void Peer::parseMessage(std::string msg)
  {
    std::cout << "length " << msg.length() << std::endl;
    for (int i = 0; i < msg.size(); ++i)
    {
      uint8_t j = msg[i];
      printf("%x\n", j);
    }

    unsigned int i = 0;
    memcpy(&i, msg.c_str(), sizeof(i));
    std::cout << "bitset : " << htons(i) << std::endl;
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

  void Peer::setFd(int fd)
  {
    fd_ = fd;
  }

} // namespace Network
