#include <iostream>
#include <stdint.h>
#include <vector>

namespace Core
{
  class Blocks
  {
    public:
      // Ctor & Dtor
      Blocks(int pieceLength);
      ~Blocks();

      bool isFull() const;
      long long int getSize() const;

    private:
      bool isFull_;
      std::vector<bool> blocks_;
      std::vector<uint8_t> data_;

  };
}
