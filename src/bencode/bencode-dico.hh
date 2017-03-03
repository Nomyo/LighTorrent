#pragma once

#include "bencode-type.hh"
#include "bencode-string.hh"

#include <map>
#include <memory>
#include <iostream>

namespace BEncode
{
  class BEncodeDictionnary : public BEncodeType
  {
  public:
    using BEncodeString_ptr = std::shared_ptr<BEncodeString>;
    using BEncodeType_ptr = std::shared_ptr<BEncodeType>;

  public:
    BEncodeDictionnary();
    BEncodeDictionnary(std::map<BEncodeString_ptr, BEncodeType_ptr> dico);

    virtual ~BEncodeDictionnary() = default;
    virtual std::string bEncode() override;
    virtual void print(std::ostream& str) const override;

    void addPair(BEncodeString_ptr key, BEncodeType_ptr data);

    friend std::ostream&
    operator<<(std::ostream& str,
	       const BEncodeDictionnary& obj);

    friend std::ostream&
    operator<<(std::ostream& str,
	       const std::shared_ptr<BEncodeDictionnary>& obj);

  private:
    std::map<BEncodeString_ptr, BEncodeType_ptr> dico_;
  };

} // namespace BEncode
