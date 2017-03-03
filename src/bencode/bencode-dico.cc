#include "bencode-dico.hh"

namespace BEncode
{
  BEncodeDictionnary::BEncodeDictionnary()
  { }

  BEncodeDictionnary::BEncodeDictionnary(std::map<BEncodeString_ptr,
					 BEncodeType_ptr> dico)
  : dico_(dico)
  { }

  std::string BEncodeDictionnary::bEncode()
  {
    value_ = "d";

    for (auto i : dico_)
    {
      value_.append(i.first->getEncodedValue());
      value_.append(i.second->getEncodedValue());
    }

    value_.append("e");

    return value_;
  }

  void BEncodeDictionnary::print(std::ostream& str) const
  {
    str << "{ ";
    int tmp = 0;
    for (auto i : dico_)
    {
      if (tmp++)
        str << ", ";
      i.first->print(str);
      str << " => ";
      i.second->print(str);
    }
    str << " }";
  }

  void BEncodeDictionnary::addPair(BEncodeString_ptr key, BEncodeType_ptr data)
  {
    dico_.emplace(std::make_pair(key, data));
  }

  std::ostream& operator<<(std::ostream& str, const BEncodeDictionnary& obj)
  {
    obj.print(str);
    return str;
  }

  std::ostream& operator<<(std::ostream& str,
			   const std::shared_ptr<BEncodeDictionnary>& obj)
  {
    obj->print(str);
    return str;
  }

} // namespace BEncode
