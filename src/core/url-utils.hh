#pragma once

#include "../bencode/bencode-dico.hh"
#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>

namespace Core
{
  class URLUtils
  {
  private:
    using BMap = std::map<std::string, std::shared_ptr<BEncode::BEncodeType>>;
    using BString = BEncode::BEncodeString;
    using BDico = BEncode::BEncodeDictionnary;
    using BType_ptr = std::shared_ptr<BEncode::BEncodeType>;

  public:
    URLUtils(BDico metaInfo);
    URLUtils(BDico metaInfo, const std::string& peerId,
	     const std::string& port);
    ~URLUtils() = default;

    void init(const BDico& metaInfo);

    std::string percentEncode(const std::string& str);

    std::string generateURL();

    template <typename T, typename R>
    R getFromMetaInfo(BDico metaInfo,
		      const std::string& field);

    template <typename T, typename R, typename U>
    U getDecode(T obj);



  private:
    std::string trackerBaseURL_;
    std::string infoHash_;
    std::string peerId_;
    std::string port_;
  };

} // namespace Core

#include "url-utils.hxx"
