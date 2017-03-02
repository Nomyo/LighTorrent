#include "bencode-dico.hh"

namespace BEncode
{
  BEncodeDictionnary::BEncodeDictionnary(const std::string& input)
    : BEncodeType(input)
  { }

  std::string BEncodeDictionnary::BEncode()
  {
    // FIXME
    return value_;
  }

} // namespace BEncode
