#include "bencode-integer.hh"
#include <string>
#include <iostream>

namespace BEncode
{
  BEncodeInteger::BEncodeInteger()
  { }

  BEncodeInteger::BEncodeInteger(long long int decode)
    : decode_(decode)
  { }

  std::string BEncodeInteger::bEncode()
  {
    value_ = "i";
    value_.append(std::to_string(decode_));
    value_.append("e");
    return value_;
  }

  void BEncodeInteger::bDecode(std::string& bufferInput)
  {

    auto pos = bufferInput.find("e");
    if (pos == std::string::npos)
    {
      std::cout << "Invalid Integer encoding" << std::endl;
      return;
    }

    bufferInput.erase(0, 1);
    auto input = bufferInput.substr(0, pos);

    int sign = 0;

    if (input[0] == '-')
    {
      ++sign;
      input = input.substr(1);
    }

    if (input[0] == '0' && (input.length() > 1 || sign))
    {
      std::cout << "Invalid integer encoding \n";
      return;
    }

    decode_ = std::stoll(input);

    if (sign)
      decode_ *= -1;

    bufferInput.erase(0, pos);
  }

  void BEncodeInteger::print(std::ostream& str) const
  {
    str << decode_;
  }

  long long int BEncodeInteger::getDecodedValue() const
  {
    return decode_;
  }

  std::ostream& operator<<(std::ostream& str, const BEncodeInteger& obj)
  {
    obj.print(str);
    return str;
  }

  std::ostream& operator<<(std::ostream& str,
			   const std::shared_ptr<BEncodeInteger>& obj)
  {
    obj->print(str);
    return str;
  }


} // namespace BEncode
