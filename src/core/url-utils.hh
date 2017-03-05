#pragma once

#include "../bencode/bencode-dico.hh"
#include "../bencode/bencode-utils.hh"

#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <openssl/sha.h>

namespace Core
{
  class URLUtils
  {
  public:
    URLUtils(BEncode::BDico metaInfo);
    URLUtils(BEncode::BDico metaInfo, const std::string& peerId,
	     const std::string& port);
    ~URLUtils() = default;

    void init(const BEncode::BDico& metaInfo);
    std::string getInfoHash(const BEncode::BDico& metaInfo);

    std::string percentEncode(const std::string& str);
    std::string generateURL();

  private:
    std::string trackerBaseUrl_;
    std::string infoHash_;
    std::string peerId_;
    std::string port_;

    /* should certainely contained in request / torrent class */
    std::string uploaded_;
    std::string downloaded_;
    std::string left_;

    std::string compat_;

  };

} // namespace Core
