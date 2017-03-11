#pragma once

#include <vector>
#include <stddef.h>

namespace Core
{
  class FileManager
  {
  public:
    FileManager();
    FileManager(size_t nbPieces);
    ~FileManager() = default;

    // Getter
    std::vector<bool> getPieces();

  private:
    std::vector<bool> pieces_;
  };

} // namespace Core
