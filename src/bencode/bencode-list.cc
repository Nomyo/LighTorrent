#include "bencode-list.hh"

namespace BEncode
{
  BEncodeList::BEncodeList(const std::string& input)
    : BEncodeType(input)
  { }

  std::string BEncodeList::BEncode()
  {
    // FIXME
    return value_;
  }

} // namespace BEncode
