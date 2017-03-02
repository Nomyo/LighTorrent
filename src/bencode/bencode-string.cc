#include "bencode-string.hh"

namespace BEncode
{
  BEncodeString::BEncodeString(const std::string& input)
    : BEncodeType(input)
  { }

  std::string BEncodeString::BEncode()
  {
    // FIXME
    return value_;
  }

} // namespace BEncode
