#ifndef LTU_FMT_PRINT_STRATEGY
#define LTU_FMT_PRINT_STRATEGY


#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <utility>


namespace ltu {

struct FmtPrintStrategy
{
  constexpr FmtPrintStrategy() = default;

  template<class... Args>
  constexpr void println(const fmt::format_string<Args...> format, Args &&...args) const noexcept
  {
    fmt::println(format, std::forward<Args>(args)...);
  }

  void println() const noexcept { fmt::println(""); }

  template<class... Args> constexpr void print(const fmt::format_string<Args...> format, Args &&...args) const noexcept
  {
    fmt::print(format, std::forward<Args>(args)...);
  }

protected:
  ~FmtPrintStrategy() = default;
};// StdPrintStrategy

}// namespace ltu

#endif// LTU_FMT_PRINT_STRATEGY
