#pragma once

#include "../bencode/bencode-dico.hh"
#include "../bencode/bencode-utils.hh"

namespace Core
{
  using namespace BEncode;

  class Torrent
  {
  public:
    Torrent(const BDico& metaInfo);
    ~Torrent() = default;

    void init();

    BDico getMetaInfo() const;
    long int getPort() const;
    long long int getUploaded() const;
    long long int getDownloaded() const;
    long long int getLeft() const;

  private:
    BDico metaInfo_;
    long int port_;
    long long int uploaded_;
    long long int downloaded_;
    long long int left_;
  };

} // namespace Core
