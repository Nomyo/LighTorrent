#include "url-utils.hh"

namespace Core
{
  URLUtils::URLUtils()
    : peerId_("-LHT01-1234568765011")
  { }


  URLUtils::URLUtils(const std::string& peerId)
    : peerId_(peerId)
  { }

  void URLUtils::init(const BDico& metaInfo)
  {
    trackerBaseUrl_ = getFromDico<BString, std::string>(metaInfo, "announce");
    infoHash_ = percentEncode(getInfoHash(metaInfo));
  }

  std::string URLUtils::percentEncode(const std::string& str)
  {
    std::stringstream tmp;
    char hexBuf[8];

    for (unsigned char i : str)
    {
      if (isalnum(i)  || (i == '.') || (i == '-')
	  || (i == '_') || (i == '~'))
	tmp << i;
      else
      {
	sprintf(hexBuf, "%X", i);
	tmp << "%";

	if (i < 16)
	  tmp << "0";

	tmp << hexBuf;
      }
    }

    return tmp.str();
  }

  std::string URLUtils::getInfoHash(const BDico& metaInfo)
  {
    BDico infDico = getType<BType_ptr, BDico>(metaInfo.get("info"));

    std::string encodedDico = infDico.bEncode();
    std::string hash;
    unsigned char outBuffer[20];
    const unsigned char *inBuffer = (const unsigned char *)encodedDico.c_str();

    SHA1(inBuffer, encodedDico.size(), outBuffer);

    for (int i = 0; i < 20; ++i) {
      hash += outBuffer[i];
    }

    return hash;
  }

  std::string URLUtils::generateURL(const Torrent& torrent)
  {
    init(torrent.getMetaInfo());

    std::stringstream url;

    url << trackerBaseUrl_  << "?";
    url << "info_hash=" << infoHash_ << "&";
    url << "peer_id=" << peerId_ << "&";
    url << "left=" << torrent.getLeft() << "&";
    url << "compact=1" << "&";
    url << "event=started";

    return url.str();
  }

} // namespace Core
