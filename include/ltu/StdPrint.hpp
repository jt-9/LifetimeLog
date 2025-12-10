#ifndef LTU_STD_PRINT_STRATEGY
#define LTU_STD_PRINT_STRATEGY

#pragma once

#include <print>
#include <utility>


namespace ltu {

struct StdPrintStrategy
{
  constexpr StdPrintStrategy() = default;

  template<class... Types>
  constexpr void println(const std::format_string<Types...> format, Types &&...args) const noexcept
  {
    std::println(format, std::forward<Types>(args)...);
  }

  void println() const noexcept { std::println(); }

  template<class... Types>
  constexpr void print(const std::format_string<Types...> format, Types &&...args) const noexcept
  {
    std::print(format, std::forward<Types>(args)...);
  }

protected:
  ~StdPrintStrategy() = default;
};// StdPrintStrategy

}// namespace ltu

#endif// LTU_STD_PRINT_STRATEGY
