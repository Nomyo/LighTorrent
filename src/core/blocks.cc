#include "blocks.hh"

namespace Core
{
  Blocks::Blocks(int pieceLength)
    : isFull_(false), data_(pieceLength)
  {
    int nbBlocks = pieceLength / 16384 + (pieceLength % 16384 == 0 ? 0 : 1);
    blocks_ = std::vector<bool>(nbBlocks);
    //std::cout << "Initialized a piece of " << blocks_.size() << " blocks. - "
    //          << pieceLength << std::endl;
  }

  Blocks::~Blocks()
  { }

  bool Blocks::isFull() const
  {
    return isFull_;
  }

  long long int Blocks::getSize() const
  {
    return data_.size();
  }
}
