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

  void FileManager::createFiles()
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
        verifyHashes();
        break;
      }
    }
  }

  void FileManager::verifyHashes()
  {
    long long int pieceLength = torrent_->getPiecesLength();
    unsigned char buffer[pieceLength] = { 0 };
    size_t bufferPos = 0;
    size_t filePos = 0;
    size_t hashCounter = 0;
    std::ifstream ifs;
    size_t fileCounter = 0;

    ifs.open(("Downloads/" + files_[fileCounter].first).c_str(), std::ios::in | std::ios::binary);
    while (fileCounter != files_.size())
    {
      char c;
      ifs.get(c);
      buffer[bufferPos] = c;
      filePos++;
      bufferPos++;
      if (bufferPos == pieceLength)
      {
        unsigned char outBuffer[20];
        SHA1(buffer, pieceLength, outBuffer);
        if (memcmp(outBuffer, hashes_[hashCounter].c_str(), 20) == 0)
          pieces_[hashCounter].setFull(true);
        hashCounter++;
        bufferPos = 0;
      }
      if (filePos == files_[fileCounter].second)
      {
        // handle file not found
        fileCounter++;
        ifs.close();
        if (fileCounter < files_.size())
          ifs.open(("Downloads/" + files_[fileCounter].first).c_str(), std::ios::in | std::ios::binary);
        filePos = 0;
      }
    }
    ifs.close();

    ColorModifier cldef(ColorCode::FG_DEFAULT);
    ColorModifier clyel(ColorCode::FG_LIGHT_YELLOW);
    size_t nbHash = 0;
    for (size_t i = 0; i < files_.size(); i++)
    {
      long long int fileSize = files_[i].second;
      size_t nbTotal = 0;
      size_t nbGood = 0;

      while (fileSize >= 0)
      {
        if (pieces_[nbHash].isFull())
          nbGood++;
        nbTotal++;
        if (fileSize >= torrent_->getPiecesLength())
          nbHash++;
        fileSize -= torrent_->getPiecesLength();
      }
      std::cout << clyel << files_[i].first << ": "
                << (nbGood * 100) / nbTotal << "%" << cldef << std::endl;
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
