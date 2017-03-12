#pragma once

#include <vector>
#include <stddef.h>
#include <bitset>
#include <strings.h>

#include "torrent.hh"
#include "blocks.hh"

namespace Core
{
  struct PieceRequest
  {
    uint32_t len;
    uint8_t id;
    uint32_t pieceIndex;
    uint32_t blockOffset;
    uint32_t blockSize;
  };

  class FileManager
  {
  public:
    FileManager();
    FileManager(Torrent *t);
    ~FileManager() = default;

    struct PieceRequest getPieceRequest(const std::vector<bool>& have);
    void setPieceRequest(const struct PieceRequest& pr, const std::string& data);

    // Getter
    std::vector<Blocks> getPieces();

  private:
    struct PieceRequest initPieceRequest() const;

    Torrent *torrent_ = nullptr;
    std::vector<Blocks> pieces_;
    std::vector<std::string> hashes_;
  };

} // namespace Core
