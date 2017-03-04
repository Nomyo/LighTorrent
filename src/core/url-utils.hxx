#include "url-utils.hh"

namespace Core
{
  template <typename T, typename R>
  R URLUtils::getFromMetaInfo(BEncode::BEncodeDictionnary metaInfo,
		    const std::string& field)
  {
    return static_cast<T*>(&*metaInfo.get(field))->getDecodedValue();
  }

  template <typename T, typename R, typename U>
  U URLUtils::getDecode(T obj)
  {
    return static_cast<R*>(&*obj)->getDecodedValue();
  }


} // namespace Core
