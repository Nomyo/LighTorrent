#include  "torrent.hh"

namespace Core
{

  Torrent::Torrent(const BDico& metaInfo)
    : metaInfo_(metaInfo)
    , port_(80)
    , uploaded_(0)
    , downloaded_(0)
    , left_(0)
  {
    init();
  }

  void Torrent::init()
  {
    BDico infoDico = getType<BType_ptr, BDico>(metaInfo_.get("info"));

    auto files = infoDico.get("files");

    if (!files)
      left_ = getDecode<BType_ptr, BInteger,
			long long int>(infoDico.get("length"));
    else
    {
      BList filesList = getType<BType_ptr, BList>(files);
      for (auto i : filesList.getDecodedValue())
      {
	BDico file = getType<BType_ptr, BDico>(i);
        left_ += getDecode<BType_ptr, BInteger,
			       long long int>(file.get("length"));
      }
    }
  }

  BDico Torrent::getMetaInfo() const
  {
    return metaInfo_;
  }

  long int Torrent::getPort() const
  {
    return port_;
  }

  long long int Torrent::getUploaded() const
  {
    return uploaded_;
  }

  long long int Torrent::getDownloaded() const
  {
    return downloaded_;
  }

  long long int Torrent::getLeft() const
  {
    return left_;
  }

} // namespace Core
