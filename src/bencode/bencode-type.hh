#pragma once

#include <string>
#include <memory>

namespace BEncode
{
  class BEncodeType
  {
  public:
    BEncodeType();

    virtual ~BEncodeType() = default;
    virtual std::string bEncode();
    virtual void bDecode(std::string& bufferInput);
    virtual void print(std::ostream& str) const;

    friend std::ostream& operator<<(std::ostream& str, const BEncodeType& obj);
    friend std::ostream& operator<<(std::ostream& str,
				    const std::shared_ptr<BEncodeType>& obj);

    std::string getEncodedValue();

  protected:
    std::string value_;
  };

} // namespace BEncode
