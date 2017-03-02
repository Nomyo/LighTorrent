#pragma once

#include "bencode-type.hh"

namespace BEncode
{
  class BEncodeString : public BEncodeType
  {
  public:

    BEncodeString(const std::string& input);
    virtual ~BEncodeString() = default;
    virtual std::string BEncode() override;

  private:
    std::string value_;
  };

} // namespace BEncode
