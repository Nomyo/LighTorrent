#include "bencode-string.hh"
#include <iostream>
#include <string>

namespace BEncode
{
  BEncodeString::BEncodeString() { }
  BEncodeString::BEncodeString(std::string decode)
    : decode_(decode)
  { }

  std::string BEncodeString::bEncode()
  {
    value_ = std::to_string(decode_.size());
    value_.append(":");
    value_.append(decode_);
    return value_;
  }

  void BEncodeString::bDecode(std::string& bufferInput)
  {
    auto pos = bufferInput.find(":");
    if (pos == std::string::npos)
    {
      std::cout << "Invalid String encoding" << std::endl;
      return;
    }

    auto len = std::stoll(bufferInput.substr(0, pos));
    decode_ = bufferInput.substr(pos + 1, len);
    bufferInput.erase(0, pos + len + 1);
  }

  void BEncodeString::print(std::ostream& str) const
  {
    str << "\"" << decode_ << "\"";
  }

  std::ostream& operator<<(std::ostream& str, const BEncodeString& obj)
  {
    obj.print(str);
    return str;
  }

  std::ostream& operator<<(std::ostream& str,
			   const std::shared_ptr<BEncodeString>& obj)
  {
    obj->print(str);
    return str;
  }

} // namespace BEncode
