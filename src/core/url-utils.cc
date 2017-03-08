#include "url-utils.hh"

namespace Core
{
  URLUtils::URLUtils()
  { }


  void URLUtils::init(const BDico& metaInfo)
  {
    trackerBaseUrl_ = getFromDico<BString, std::string>(metaInfo, "announce");
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

  std::string URLUtils::generateURL(const Network::Torrent& torrent)
  {
    init(torrent.getMetaInfo());
    std::stringstream url;

    url << trackerBaseUrl_  << "?";
    url << "info_hash=" << percentEncode(torrent.getInfoHash()) << "&";
    url << "peer_id=" << torrent.getPeerId() << "&";
    url << "left=" << torrent.getLeft() << "&";
    url << "compact=1" << "&";
    url << "event=started";

    return url.str();
  }

} // namespace Core
