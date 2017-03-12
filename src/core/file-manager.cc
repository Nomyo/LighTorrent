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

    files_ = t->getFiles();

    long long int left = t->getLeft();
    while (left > 0)
    {
      long long int pieceSize = left > t->getPiecesLength() ? t->getPiecesLength() : left;
      pieces_.push_back(Blocks(pieceSize));
      left -= pieceSize;
    }

    createFiles();

    std::cout << "total file size = " << t->getLeft() << std::endl;
    std::cout << "Initialized FileManager with " << t->getNbPieces()
      << " pieces of size ~" << t->getPiecesLength() << std::endl;
    std::cout << "Pieces Hash length = " << t->getPiecesHash().length() << std::endl;
  }

  struct PieceRequest FileManager::getPieceRequest(const std::vector<bool>& have)
  {
    if (have.size() != pieces_.size())
      std::cerr << "Error, peer have is != from torrent have" << std::endl;

    struct PieceRequest req = initPieceRequest();
    for (size_t i = 0; i < have.size(); i++)
    {
      if (have[i] && !pieces_[i].isFull() && !pieces_[i].isWaiting())
      {
        uint32_t blockOffset = pieces_[i].getBlockOffset();
        req.pieceIndex = i;
        req.blockOffset = blockOffset;
        req.blockSize = 16384;
        break;
      }
    }

    return req;
  }

  void FileManager::setPieceRequest(const struct PieceRequest& pr, const std::string& data)
  {
    pieces_[pr.pieceIndex].setBlockData(pr.blockOffset, data);
  }

  void FileManager::createFiles() const
  {
    for (size_t i = 0; i < files_.size(); i++)
    {
      struct stat buffer;
      if (stat(("Downloads/" + files_[i].first).c_str(), &buffer) != 0) // file doesn't exist
      {
        mkdir("Downloads", 0777);
        std::ofstream ofs("Downloads/" + files_[i].first, std::ios::binary | std::ios::out);
        ofs.seekp(files_[i].second - 1);
        ofs.write("", 1);
        ofs.close();
        std::cout << "Created file " << files_[i].first << std::endl;
      }
      else
      {
        std::cout << "File " << files_[i].first << " already exists." << std::endl;
      }
    }
  }

  struct PieceRequest FileManager::initPieceRequest() const
  {
    struct PieceRequest req;
    bzero(&req, sizeof (req));
    req.len = 13;
    req.id = 6;
    return req;
  }

  std::vector<Blocks> FileManager::getPieces()
  {
    return pieces_;
  }
} // namespace Core
