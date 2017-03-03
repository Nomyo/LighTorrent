#pragma once

#include "bencode-type.hh"

namespace BEncode
{
  class BEncodeInteger : public BEncodeType
  {
  public:
    BEncodeInteger();
    BEncodeInteger(long long int decode);

    virtual ~BEncodeInteger() = default;
    virtual std::string bEncode() override;
    virtual void bDecode(std::string& bufferInput) override;
    virtual void print(std::ostream& str) const override;

    friend std::ostream& operator<<(std::ostream& str,
				    const BEncodeInteger& obj);
    friend std::ostream& operator<<(std::ostream& str,
				    const std::shared_ptr<BEncodeInteger>& obj);

  private:
    long long int decode_;
  };

} // namespace BEncode
