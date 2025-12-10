#ifndef LTU_FMT_FORMATTER
#define LTU_FMT_FORMATTER

#pragma once

#include <fmt/format.h>

namespace ltu {
template<typename T, class PrintStrategy> struct LftLog;
}

// namespace ltu {

template<class CharT, typename Type, class PrintStrategy>
// requires(!std::is_void_v<Type>)
struct fmt::formatter<ltu::LftLog<Type, PrintStrategy>, CharT> : fmt::formatter<Type, CharT>
{
  static_assert(!std::is_void_v<Type>, "Type 'void' is not formattable");

  template<typename FormatContext> constexpr FormatContext::iterator format(const Type &value, FormatContext &ctx) const
  {
    return fmt::formatter<Type, CharT>::format(value, ctx);
  }
};

//}// namespace ltu

#endif// LTU_FMT_FORMATTER
