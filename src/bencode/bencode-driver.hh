#pragma once

#include "bencode-type.hh"

namespace BEncode
{
  class BEncodeDriver
  {
  public:
    BEncodeDriver();
    ~BEncodeDriver() = default;

    BEncodeType BDecode(std::string bufferInput);
    std::string BEncode(BEncodeType node);

  };

} // namespace BEncode
