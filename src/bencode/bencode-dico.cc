#include "bencode-dico.hh"

namespace BEncode
{
  BEncodeDictionnary::BEncodeDictionnary()
  { }

  BEncodeDictionnary::BEncodeDictionnary(std::map<std::string,
					 BEncodeType_ptr> dico)
  : dico_(dico)
  { }

  std::string BEncodeDictionnary::bEncode()
  {
    value_ = "d";

    for (auto i : dico_)
    {
      value_.append(i.first);
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
      str << i.first << " => ";
      i.second->print(str);
    }
    str << " }";
  }

  void BEncodeDictionnary::addPair(std::string key, BEncodeType_ptr data)
  {
    dico_.emplace(std::make_pair(key, data));
  }

  std::shared_ptr<BEncodeType> BEncodeDictionnary::get(const std::string& key)
  {
    auto pair = dico_.find(key);
    if (pair != dico_.end())
      return dico_.find(key)->second;

    return nullptr;
  }

  auto BEncodeDictionnary::getDecodedValue() const ->
    std::map<std::string, BEncodeType_ptr>
  {
    return dico_;
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
