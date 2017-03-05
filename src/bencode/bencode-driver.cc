#include "bencode-driver.hh"

namespace BEncode
{
  BEncodeDriver::BEncodeDriver() { }

  std::shared_ptr<BEncodeType>
  BEncodeDriver::bDecodeFile(const std::string& filename)
  {
    std::ifstream file(filename);

    if (!file.is_open())
    {
      std::cout << "Couldn't open file :" << filename << std::endl;
      return nullptr;
    }

    std::string result;
    std::string line;

    while (std::getline(file, line))
    {
      result += line;
      result += "\n";
    }

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

    while (bufferInput[0] != 'e' && bufferInput.size() != 0)
      node->addBType(bDecode(bufferInput));

    if (bufferInput.size() == 0)
      std::cout << "decoding failed" << std::endl;
    else
      bufferInput.erase(0, 1);

    return node;
  }

  std::shared_ptr<BEncodeDictionnary>
  BEncodeDriver::bDecodeDico(std::string& bufferInput)
  {
    auto node = std::make_shared<BEncodeDictionnary>();
    bufferInput.erase(0, 1);

    while (bufferInput[0] != 'e' && bufferInput.size() != 0)
    {
      auto key = bDecodeString(bufferInput)->getDecodedValue();
      auto data = bDecode(bufferInput);
      node->addPair(key, data);
    }

    if (bufferInput.size() == 0)
      std::cout << "decoding failed" << std::endl;
    else
      bufferInput.erase(0, 1);

    return node;
  }

} // namespace BEncode
