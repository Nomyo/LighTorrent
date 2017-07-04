#pragma once

#include <vector>
#include <memory>

#include "bencode/bencode-type.hh"

namespace BEncode
{
  class BEncodeList : public BEncodeType
  {
  public:
    BEncodeList();
    BEncodeList(std::vector<std::shared_ptr<BEncodeType>> list);

    virtual ~BEncodeList() = default;
    virtual std::string bEncode() override;
    virtual void print(std::ostream& str) const override;

    void addBType(std::shared_ptr<BEncodeType> ptr);

    friend std::ostream& operator<<(std::ostream& str, const BEncodeList& obj);
    friend std::ostream& operator<<(std::ostream& str,
				    const std::shared_ptr<BEncodeList>& obj);

    std::vector<std::shared_ptr<BEncodeType>> getDecodedValue() const;

  private:
    std::vector<std::shared_ptr<BEncodeType>> list_;
  };

} // namespace BEncode
