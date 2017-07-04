#pragma once

#include "bencode-type.hh"
#include "bencode-string.hh"

#include "core/color.hh"

#include <map>
#include <memory>
#include <iostream>
#include <exception>

namespace BEncode
{
  class BEncodeDictionnary : public BEncodeType
  {
  public:
    using BEncodeType_ptr = std::shared_ptr<BEncodeType>;

  public:
    BEncodeDictionnary();
    BEncodeDictionnary(std::map<std::string, BEncodeType_ptr> dico);

    virtual ~BEncodeDictionnary() = default;
    virtual std::string bEncode() override;
    virtual void print(std::ostream& str) const override;

    void addPair(std::string key, BEncodeType_ptr data);
    std::shared_ptr<BEncodeType> get(const std::string& key) const;

    std::map<std::string, BEncodeType_ptr> getDecodedValue() const;

    friend std::ostream&
    operator<<(std::ostream& str,
	       const BEncodeDictionnary& obj);

    friend std::ostream&
    operator<<(std::ostream& str,
	       const std::shared_ptr<BEncodeDictionnary>& obj);

  private:
    std::map<std::string, BEncodeType_ptr> dico_;
  };

} // namespace BEncode
