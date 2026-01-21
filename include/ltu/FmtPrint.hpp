#ifndef LTU_FMT_PRINT_STRATEGY
#define LTU_FMT_PRINT_STRATEGY


#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <utility>


namespace ltu {

struct FmtPrintStrategy
{
  template<class... Args>
  static constexpr void println(const fmt::format_string<Args...> format, Args &&...args) noexcept
  {
    fmt::println(format, std::forward<Args>(args)...);
  }

  static void println() noexcept { fmt::println(""); }

  template<class... Args> static constexpr void print(const fmt::format_string<Args...> format, Args &&...args) noexcept
  {
    fmt::print(format, std::forward<Args>(args)...);
  }

protected:
  ~FmtPrintStrategy() = default;
};// StdPrintStrategy

}// namespace ltu

#endif// LTU_FMT_PRINT_STRATEGY
