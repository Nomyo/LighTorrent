#pragma once

#include "bencode-type.hh"

namespace BEncode
{
  class BEncodeInteger : public BEncodeType
  {
  public:

    BEncodeInteger(const std::string& input);
    virtual ~BEncodeInteger() = default;
    virtual std::string BEncode() override;

  private:
    std::string value_;
  };

} // namespace BEncode
