#pragma once

#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>

#include "bencode/bencode-dico.hh"
#include "bencode/bencode-utils.hh"

#include "torrent.hh"

namespace Core
{
  using namespace BEncode;
  class URLUtils
  {
  public:
    URLUtils();
    ~URLUtils() = default;

    void init(const BDico& metaInfo);

    std::string percentEncode(const std::string& str);
    std::string generateURL(const Torrent& torrent);

  private:
    std::string trackerBaseUrl_;
  };

} // namespace Core
