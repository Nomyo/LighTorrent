#include "bencode/bencode-dico.hh"
#include "bencode/bencode-string.hh"
#include "bencode/bencode-integer.hh"
#include "bencode/bencode-list.hh"

namespace BEncode
{
  using BMap = std::map<std::string, std::shared_ptr<BEncodeType>>;
  using BDico = BEncodeDictionnary;
  using BList = BEncodeList;
  using BString = BEncodeString;
  using BInteger = BEncodeInteger;
  using BType = BEncodeType;
  using BType_ptr = std::shared_ptr<BEncodeType>;

} // namespace BEncode
