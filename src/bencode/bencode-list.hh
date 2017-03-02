#pragma once

#include "bencode-type.hh"
#include <vector>

namespace BEncode
{
  class BEncodeList : public BEncodeType
  {
  public:

    BEncodeList(const std::string& input);
    virtual ~BEncodeList() = default;
    virtual std::string BEncode() override;

  private:
    std::vector<BEncodeType> list_;
    std::string value_;
  };

} // namespace BEncode
