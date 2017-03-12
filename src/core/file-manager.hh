#pragma once

#include <vector>
#include <stddef.h>
#include <bitset>

#include "torrent.hh"
#include "blocks.hh"

namespace Core
{
  class FileManager
  {
  public:
    FileManager();
    FileManager(Torrent *t);
    ~FileManager() = default;

    // Getter
    std::vector<Blocks> getPieces();

  private:
    Torrent *torrent_ = nullptr;
    std::vector<Blocks> pieces_;
    std::vector<std::string> hashes_;
  };

} // namespace Core
