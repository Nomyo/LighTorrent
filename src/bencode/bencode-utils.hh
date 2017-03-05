#pragma once

#include "fwd.hh"

namespace BEncode
{
  template <typename T, typename R>
  R getFromDico(const BDico& metaInfo,
		    const std::string& field);

  template <typename T, typename R, typename U>
  U getDecode(T obj);

} // namespace BEncode

#include "bencode-utils.hxx"
