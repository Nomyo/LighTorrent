#pragma once

#include <string>

namespace BEncode
{
  class BEncodeType
  {
  public:

    BEncodeType(const std::string& input);
    virtual ~BEncodeType() = default;
    virtual std::string BEncode();

  private:
    std::string input_;
  };

} // namespace BEncode
