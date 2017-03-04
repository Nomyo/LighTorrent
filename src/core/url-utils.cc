#include "url-utils.hh"

#include <memory>
#include <iostream>
#include <fstream>

namespace Core
{
  URLUtils::URLUtils(BDico metaInfo)
  {
    trackerBaseURL_ = getFromMetaInfo<BString,
				      std::string>(metaInfo, "announce");

    auto dInfo = getFromMetaInfo<BDico, BMap>(metaInfo, "info");
    auto tmp = dInfo.find("pieces");
    if (tmp == dInfo.end())
    {
      std::cout << "Corrumpted meta info header" << std::endl;
      return;
    }

    infoHash_ = percentEncode(getDecode<BType_ptr,
			      BString, std::string>(tmp->second));

    std::cout << "Track = " << trackerBaseURL_ << std::endl;
    std::cout << "Hash = " << infoHash_ << std::endl;
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

  std::string URLUtils::generateURL()
  {
    return "";
  }

} // namespace Core
