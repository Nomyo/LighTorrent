#include "bencode-driver.hh"

namespace BEncode
{
  BEncodeDriver::BEncodeDriver() { }

  std::shared_ptr<BEncodeType>
  BEncodeDriver::bDecodeFile(const std::string& filename)
  {
    std::ifstream file(filename);
    std::string result;
    std::getline(file, result);

    return bDecode(result);
  }

  std::shared_ptr<BEncodeType>
  BEncodeDriver::bDecode(std::string& bufferInput)
  {
    switch (bufferInput[0])
    {
    case 'i':
    {
      return bDecodeInteger(bufferInput);
    }
    case '0'...'9':
    {
      return bDecodeString(bufferInput);
    }
    case 'l':
    {
      return bDecodeList(bufferInput);
    }
    case 'd':
    {
      return bDecodeDico(bufferInput);
    }

    default:
      break;
    }

    return std::make_shared<BEncodeType>();
  }

  std::shared_ptr<BEncodeInteger>
  BEncodeDriver::bDecodeInteger(std::string& bufferInput)
  {
    auto node = std::make_shared<BEncodeInteger>();
    node->bDecode(bufferInput);
    return node;
  }

  std::shared_ptr<BEncodeString>
  BEncodeDriver::bDecodeString(std::string& bufferInput)
  {
    auto node = std::make_shared<BEncodeString>();
    node->bDecode(bufferInput);
    return node;
  }


  std::shared_ptr<BEncodeList>
  BEncodeDriver::bDecodeList(std::string& bufferInput)
  {
    auto node = std::make_shared<BEncodeList>();
    bufferInput.erase(0, 1);

    while (bufferInput[0] != 'e')
      node->addBType(bDecode(bufferInput));

    bufferInput.erase(0, 1);
    return node;
  }

  std::shared_ptr<BEncodeDictionnary>
  BEncodeDriver::bDecodeDico(std::string& bufferInput)
  {
    auto node = std::make_shared<BEncodeDictionnary>();
    bufferInput.erase(0, 1);

    while (bufferInput[0] != 'e')
    {
      auto key = bDecodeString(bufferInput)->getDecodedValue();
      auto data = bDecode(bufferInput);
      node->addPair(key, data);
    }

    bufferInput.erase(0, 1);
    return node;
  }

} // namespace BEncode
