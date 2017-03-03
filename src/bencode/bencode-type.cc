#include "bencode-type.hh"

namespace BEncode
{

  BEncodeType::BEncodeType()
  { }

  std::string BEncodeType::bEncode()
  {
    return "";
  }

  void BEncodeType::bDecode(std::string&)
  { }

  void BEncodeType::print(std::ostream&) const
  { }

  std::string BEncodeType::getEncodedValue()
  {
    return value_;
  }

  std::ostream& operator<<(std::ostream& str, const BEncodeType& obj)
  {
    obj.print(str);
    return str;
  }

  std::ostream& operator<<(std::ostream& str,
			   const std::shared_ptr<BEncodeType>& obj)
  {
    obj->print(str);
    return str;
  }

} // namespace BEncode
