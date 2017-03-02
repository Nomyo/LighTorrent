#pragma once

#include "bencode-type.hh"
#include "bencode-string.hh"
#include <map>

namespace BEncode
{
  class BEncodeDictionnary : public BEncodeType
  {
  public:

    BEncodeDictionnary(const std::string& input);
    virtual ~BEncodeDictionnary() = default;
    virtual std::string BEncode() override;

  private:
    std::map<BEncodeString, BEncodeType> dico_;
    std::string value_;
  };

} // namespace BEncode
