#include "blocks.hh"

namespace Core
{
  Blocks::Blocks(int pieceLength)
    : isFull_(false), isWaiting_(false)
  {
    int nbBlocks = pieceLength / BLOCKSIZE + (pieceLength % BLOCKSIZE == 0 ? 0 : 1);
    blocks_ = std::vector<bool>(nbBlocks);
    requested_ = std::vector<bool>(nbBlocks);
    data_ = std::vector<std::string>(nbBlocks);
  }

  Blocks::~Blocks()
  { }

  int Blocks::getBlockOffset()
  {
    if (isFull_)
      return -1;
    for (size_t i = 0; i < blocks_.size(); i++)
    {
      if (!blocks_[i] && !requested_[i])
      {
        requested_[i] = true;
        if (i == blocks_.size() - 1)
          isWaiting_ = true;
        return i;
      }
    }
    return -1;
  }

  void Blocks::setBlockData(uint32_t block, const std::string& data)
  {
    data_[block] = data;
    blocks_[block] = true;
    requested_[block] = false;

    size_t firstFalse = 0;
    while (firstFalse < blocks_.size() && !blocks_[firstFalse])
      firstFalse++;

    if (firstFalse == blocks_.size())
      isFull_ = true;
  }

  bool Blocks::isFull() const
  {
    return isFull_;
  }

  void Blocks::setFull(bool full)
  {
    isFull_ = full;
  }

  bool Blocks::isWaiting() const
  {
    return isWaiting_;
  }

  long long int Blocks::getSize() const
  {
    return data_.size();
  }
}
