#include "peer.hh"
#include "../core/color.hh"

namespace Network
{

  Peer::Peer(const std::string& ip, in_port_t port)
    : ip_(ip)
    , port_(port)
  { }

  std::string Peer::getIp()
  {
    return ip_;
  }

  in_port_t Peer::getPort()
  {
    return port_;
  }

  void Peer::dump()
  {
    Core::ColorModifier clgreen(Core::ColorCode::FG_LIGHT_BLUE);
    Core::ColorModifier clblue(Core::ColorCode::FG_BLUE);
    Core::ColorModifier cdef(Core::ColorCode::FG_DEFAULT);

    std::cout << clblue << ip_  << " " << cdef << " "
	      << clgreen << port_ << cdef << std::endl;
  }

} // namespace Network
