#ifndef LTU_FMT_FORMATTER
#define LTU_FMT_FORMATTER


#include <fmt/base.h>
// #include <fmt/format.h>

#include <type_traits>


namespace ltu {
template<typename T, class PrintStrategy, class MemFunDataFormatter> struct LftLog;
}


template<class CharT, typename Type, class PrintStrategy, class MemFunDataFormatter>
#ifdef __cpp_concepts
  requires fmt::formattable<Type, CharT>
#endif
struct fmt::formatter<ltu::LftLog<Type, PrintStrategy, MemFunDataFormatter>, CharT>
  : fmt::formatter<std::remove_cvref_t<Type>, CharT>
{
  static_assert(!std::is_void_v<Type>, "Type 'void' is not formattable");

  template<typename FormatContext>
  constexpr typename FormatContext::iterator format(const Type &value, FormatContext &ctx) const
  {
    return fmt::formatter<std::remove_cvref_t<Type>, CharT>::format(value, ctx);
  }
};


#endif// LTU_FMT_FORMATTER
