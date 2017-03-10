#pragma once

#include <stddef.h>
#include <stdlib.h>

namespace Core
{
  class Message
  {

  public:
    // Ctor & Dtor
    Message(char *buffer, size_t length)
      : buffer_(buffer)
      , length_(length)
      { }

    ~Message() { free(buffer_); };

    // Getters
    char *getBuffer() const { return buffer_; }
    size_t getLength() const { return length_; }

  private:
    char *buffer_;
    size_t length_;
  };

} // namespace Core
