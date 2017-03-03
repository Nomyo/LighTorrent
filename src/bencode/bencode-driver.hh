#pragma once

#include "bencode-type.hh"
#include "bencode-integer.hh"
#include "bencode-string.hh"
#include "bencode-list.hh"
#include "bencode-dico.hh"

#include <memory>
#include <iostream>
#include <fstream>

namespace BEncode
{
  class BEncodeDriver
  {
  public:
    BEncodeDriver();
    ~BEncodeDriver() = default;


    std::shared_ptr<BEncodeType> bDecodeFile(const std::string& filename);
    std::shared_ptr<BEncodeType> bDecode(std::string& bufferInput);

    std::shared_ptr<BEncodeInteger> bDecodeInteger(std::string& bufferInput);
    std::shared_ptr<BEncodeString> bDecodeString(std::string& bufferInput);
    std::shared_ptr<BEncodeList> bDecodeList(std::string& bufferInput);
    std::shared_ptr<BEncodeDictionnary> bDecodeDico(std::string& bufferInput);

  };

} // namespace BEncode
