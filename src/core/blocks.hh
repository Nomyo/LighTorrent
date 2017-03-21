#include <iostream>
#include <stdint.h>
#include <vector>

#define BLOCKSIZE 16384

namespace Core
{
  class Blocks
  {
    public:
      // Ctor & Dtor
      Blocks(int pieceLength);
      ~Blocks();

      int getBlockOffset();
      void setBlockData(uint32_t block, const std::string& data);

      bool isFull() const;
      void setFull(bool full);
      bool isWaiting() const;
      long long int getSize() const;
      const std::vector<std::string>& getData() const;

    private:
      bool isFull_;
      bool isWaiting_; // waiting for return of peer messages (requested is full)
      std::vector<bool> blocks_;
      std::vector<bool> requested_;
      std::vector<std::string> data_;
  };
}
