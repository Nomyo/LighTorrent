#include "file-manager.hh"

namespace Core
{
  FileManager::FileManager()
  { }

  FileManager::FileManager(Torrent *t)
    : torrent_(t)
  {
        const std::string& pieces = t->getPiecesHash();
    for (size_t i = 0; i < t->getNbPieces(); i++)
      hashes_.push_back(pieces.substr(i * 20, 20));

    long long int left = t->getLeft();

    while (left > 0)
    {
      long long int pieceSize = left > t->getPiecesLength() ? t->getPiecesLength() : left;
      pieces_.push_back(Blocks(pieceSize));
      left -= pieceSize;
    }

    std::cout << "total file size = " << t->getLeft() << std::endl;
    long long int blocksSize = 0;
    for (auto piece: pieces_)
      blocksSize += piece.getSize();
    std::cout << "total blocks size = " << blocksSize << std::endl;
    std::cout << "Initialized FileManager with " << t->getNbPieces()
              << " pieces of size ~" << t->getPiecesLength() << std::endl;
    std::cout << "Pieces Hash length = " << t->getPiecesHash().length() << std::endl;
  }

  std::vector<Blocks> FileManager::getPieces()
  {
    return pieces_;
  }

} // namespace Core
