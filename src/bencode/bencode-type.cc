#include "bencode-type.hh"

namespace BEncode
{
  BEncodeType::BEncodeType(const std::string& input)
    : input_(input)
  { }

  std::string BEncodeType::BEncode()
  {
    // Default behaviour no encoding done
    return input_;
  }

} // namespace BEncode
