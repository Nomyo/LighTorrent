#include "bencode-list.hh"
#include <iostream>

namespace BEncode
{
  BEncodeList::BEncodeList()
  { }

  BEncodeList::BEncodeList(std::vector<std::shared_ptr<BEncodeType>> list)
    : list_(list)
  { }

  std::string BEncodeList::bEncode()
  {
    value_ = "l";

    for (auto i : list_)
      value_.append(i->getEncodedValue());

    value_.append("e");

    return value_;
  }

  void BEncodeList::print(std::ostream& str) const
  {
    int tmp = 0;
    str << "[";
    for (auto i : list_)
    {
      if (tmp++)
        str << ", ";
      i->print(str);
    }
    str << "]";
  }

  void BEncodeList::addBType(std::shared_ptr<BEncodeType> ptr)
  {
    list_.push_back(ptr);
  }

  std::vector<std::shared_ptr<BEncodeType>>
  BEncodeList::getDecodedValue() const
  {
    return list_;
  }

  std::ostream& operator<<(std::ostream& str, const BEncodeList& obj)
  {
    obj.print(str);
    return str;
  }

  std::ostream& operator<<(std::ostream& str,
			   const std::shared_ptr<BEncodeList>& obj)
  {
    obj->print(str);
    return str;
  }

} // namespace BEncode
