#include "bencode-utils.hh"

namespace BEncode
{

  template <typename T, typename R>
  R getFromDico(const BEncode::BEncodeDictionnary& metaInfo,
		const std::string& field)
  {
    return static_cast<T*>(&*metaInfo.get(field))->getDecodedValue();
  }

  template <typename T, typename R, typename U>
  U getDecode(T obj)
  {
    return static_cast<R*>(&*obj)->getDecodedValue();
  }

  template <typename T, typename R>
  R getType(const T& obj)
  {
    return *static_cast<R*>(&*obj);
  }

} // namespace BEncode
