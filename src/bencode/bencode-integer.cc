#include "bencode-integer.hh"

namespace BEncode
{
  BEncodeInteger::BEncodeInteger(const std::string& input)
    : BEncodeType(input)
  { }

  std::string BEncodeInteger::BEncode()
  {
    // FIXME
    return value_;
  }

} // namespace BEncode
