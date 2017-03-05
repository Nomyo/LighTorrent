#include "url-utils.hh"

namespace Core
{
  URLUtils::URLUtils(BEncode::BDico metaInfo)
    : peerId_("-LHT01-1234568765011")
    , port_("7689")
    , downloaded_("0")
    , left_("1000000")
  {
    init(metaInfo);
  }


  URLUtils::URLUtils(BEncode::BDico metaInfo, const std::string& peerId,
		     const std::string& port)
    : peerId_(peerId)
    , port_(port)
    , downloaded_("0")
    , left_("1000000")
  {
    init(metaInfo);
  }

  void URLUtils::init(const BEncode::BDico& metaInfo)
  {
    trackerBaseUrl_ = BEncode::getFromDico<BEncode::BString,
					   std::string>(metaInfo, "announce");

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

  std::string URLUtils::getInfoHash(const BEncode::BDico& metaInfo)
  {
    BEncode::BDico infDico =
      BEncode::getType<BEncode::BType_ptr, BEncode::BDico>(metaInfo.get("info"));

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

  std::string URLUtils::generateURL()
  {
    std::stringstream url;
    url << trackerBaseUrl_  << "?";
    url << "info_hash=" << infoHash_ << "&";
    url << "peer_id=" << peerId_ << "&";
    url << "ip=" << "255.255.255.255" << "&";
    url << "port=" << port_ << "&";
    url << "downloaded=" << downloaded_ << "&";
    url << "left=" << left_ << "&";
    url << "event=started";

    return url.str();
  }

} // namespace Core
