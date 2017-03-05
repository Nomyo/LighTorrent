#pragma once

#include "../bencode/bencode-dico.hh"
#include "../bencode/bencode-utils.hh"
#include "torrent.hh"

#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <openssl/sha.h>

namespace Core
{
  using namespace BEncode;
  class URLUtils
  {
  public:
    URLUtils();
    URLUtils(const std::string& peerId);
    ~URLUtils() = default;

    void init(const BDico& metaInfo);
    std::string getInfoHash(const BDico& metaInfo);

    std::string percentEncode(const std::string& str);
    std::string generateURL(const Torrent& torrent);

  private:
    std::string trackerBaseUrl_;
    std::string infoHash_;
    std::string peerId_;
  };

} // namespace Core
