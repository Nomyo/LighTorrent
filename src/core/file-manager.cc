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
    directory_ = extractFileName(t->getFileName());

    long long int left = t->getLeft();
    while (left > 0)
    {
      long long int pieceSize = left > t->getPiecesLength() ? t->getPiecesLength() : left;
      pieces_.push_back(Blocks(pieceSize));
      left -= pieceSize;
    }

    initDirectory();
    dumpPieces();

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
      // If the block has not been downloaded yet and is now waiting for a response
      if (have[i] && !pieces_[i].isFull() && !pieces_[i].isWaiting())
      {
        uint32_t blockOffset = pieces_[i].getBlockOffset();
        req.pieceIndex = i;
        req.blockOffset = blockOffset;
        req.blockSize = BLOCKSIZE;
        break;
      }
    }

    return req;
  }

  void FileManager::setPieceRequest(const struct PieceRequest& pr, const std::string& data)
  {
    pieces_[pr.pieceIndex].setBlockData(pr.blockOffset, data);
    if (pieces_[pr.pieceIndex].isFull())
    {
      // The length of a piece (in bits), also, the size to write
      long long int pLen = torrent_->getPiecesLength();
      // The cursor position in all the data (in bits)
      long long int cPos = pr.pieceIndex * pLen + pr.blockOffset * pr.blockSize;
      // The index of the file in which we write
      int fileIndex = 0;
      // The offset of the data vector in the piece we extract
      int dataPos = 0;
      // The data vector to write
      const std::vector<std::string>& data = pieces_[pr.pieceIndex].getData();

      // We find the file to fill
      while (cPos > files_[fileIndex].second)
      {
        cPos -= files_[fileIndex].second;
        fileIndex++;
      }

      // While we have something to write
      while (pLen > 0)
      {
        std::ofstream ofs(directory_ + "/" + files_[fileIndex].first, std::ios::binary | std::ios::out);
        ofs.seekp(cPos);

        // If the data to write is bigger than the file to write it in
        if (files_[fileIndex].second - cPos < pLen)
        {
          // The maximum data we can write (in bits)
          long long int sizeToWrite = files_[fileIndex].second - cPos;
          std::string res;
          for (size_t i = dataPos; i < data.size(); i++)
            res += data[i];

          ofs.write(res.c_str(), sizeToWrite / 8);

          ofs.close();
          fileIndex++;
          ofs.open(directory_ + "/" + files_[fileIndex].first, std::ios::binary | std::ios::out);
          dataPos += sizeToWrite / 8;
          cPos = 0;
          pLen -= sizeToWrite;
        }
        else
        {
          std::string res;
          for (size_t i = dataPos; i < data.size(); i++)
            res += data[i];

          ofs.write(res.c_str(), pLen / 8);
          ofs.close();
          pLen = 0;
        }
      }

      std::cout << "Wrote a block of data to file." << std::endl;
    }
  }

  void FileManager::initDirectory()
  {
    struct stat buffer;
    if (stat((directory_ + "/").c_str(), &buffer) != 0) // directory doesn't exist
    {
      mkdir(directory_.c_str(), 0777);
      for (size_t i = 0; i < files_.size(); i++)
      {
        std::ofstream ofs(directory_ + "/" + files_[i].first, std::ios::binary | std::ios::out);
        ofs.seekp(files_[i].second - 1);
        ofs.write("", 1);
        ofs.close();
        std::cout << "Created file " << files_[i].first << std::endl;
      }
    }
    else
      verifyHashes();
  }

  void FileManager::verifyHashes()
  {
    std::cout << "Verifying hashes...\r" << std::flush;
    long long int pieceLength = torrent_->getPiecesLength();
    unsigned char buffer[pieceLength] = { 0 };
    int bufferPos = 0;
    int filePos = 0;
    size_t hashCounter = 0;
    std::ifstream ifs;
    size_t fileCounter = 0;

    ifs.open((directory_ + "/" + files_[fileCounter].first).c_str(), std::ios::in | std::ios::binary);
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
        std::cout << "Verifying hashes..." << (hashCounter * 100) / pieces_.size()
                  << "%\r" << std::flush;
      }
      if (filePos == files_[fileCounter].second)
      {
        // handle file not found
        fileCounter++;
        ifs.close();
        if (fileCounter < files_.size())
          ifs.open((directory_ + "/" + files_[fileCounter].first).c_str(), std::ios::in | std::ios::binary);
        filePos = 0;
      }
    }
    ifs.close();
  }

  struct PieceRequest FileManager::initPieceRequest() const
  {
    struct PieceRequest req;
    bzero(&req, sizeof(req));
    req.len = 13;
    req.id = 6;
    return req;
  }

  std::string FileManager::extractFileName(const std::string& fullPath) const
  {
    int counterDot = fullPath.length() - 1;
    while (fullPath[counterDot] != '.')
      counterDot--;
    counterDot -= 1;

    int counterSlash = counterDot;
    while (counterSlash >= 0 && fullPath[counterSlash] != '/')
      counterSlash--;

    return fullPath.substr(counterSlash + 1, counterDot - counterSlash);
  }

  void FileManager::dumpPieces() const
  {
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

  std::vector<Blocks> FileManager::getPieces()
  {
    return pieces_;
  }
} // namespace Core
