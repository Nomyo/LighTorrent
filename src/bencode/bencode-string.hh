#pragma once

#include "bencode/bencode-type.hh"

namespace BEncode
{
  class BEncodeString : public BEncodeType
  {
  public:
    BEncodeString();
    BEncodeString(std::string decode);

    virtual ~BEncodeString() = default;
    virtual std::string bEncode() override;
    virtual void bDecode(std::string& bufferInput) override;
    virtual void print(std::ostream& str) const override;

    friend std::ostream& operator<<(std::ostream& str, const BEncodeString& obj);
    friend std::ostream& operator<<(std::ostream& str,
				    const std::shared_ptr<BEncodeString>& obj);

    std::string getDecodedValue() const;

  private:
    std::string decode_;
  };

} // namespace BEncode
