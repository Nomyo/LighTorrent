#include "file-manager.hh"

namespace Core
{
  FileManager::FileManager()
  { }

  FileManager::FileManager(size_t nbPieces)
    : pieces_(nbPieces)
  { }

  std::vector<bool> FileManager::getPieces()
  {
    return pieces_;
  }

} // namespace Core
