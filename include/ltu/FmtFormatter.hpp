#ifndef LTU_FMT_FORMATTER
#define LTU_FMT_FORMATTER


#include <fmt/base.h>
// #include <fmt/format.h>

#include <type_traits>


namespace ltu {
template<typename T, class PrintStrategy, class MemDataFormatter> struct LftLog;
}


template<class CharT, typename Type, class PrintStrategy, class MemDataFormatter>
  requires fmt::formattable<Type, CharT>
struct fmt::formatter<ltu::LftLog<Type, PrintStrategy, MemDataFormatter>, CharT>
  : fmt::formatter<std::remove_cvref_t<Type>, CharT>
{
  static_assert(!std::is_void_v<Type>, "Type 'void' is not formattable");

private:
  using maybe_const_LftLog = std::conditional_t<fmt::formattable<const Type, CharT>,
    const ltu::LftLog<Type, PrintStrategy, MemDataFormatter>,
    ltu::LftLog<Type, PrintStrategy, MemDataFormatter>>;

public:
  template<typename FormatContext>
  constexpr typename FormatContext::iterator format(maybe_const_LftLog &value, FormatContext &ctx) const
  {
    return fmt::formatter<std::remove_cvref_t<Type>, CharT>::format(value.t_, ctx);
  }
};


#endif// LTU_FMT_FORMATTER
