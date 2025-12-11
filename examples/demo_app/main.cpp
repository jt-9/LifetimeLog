// #include "ltu/FmtFormatter.hpp"
// #include "ltu/FmtPrint.hpp"
#include "ltu/LftLog.hpp"

// NOLINTNEXTLINE
#include <print>
// #include <fmt/base.h>


int main(/*int argc, char **argv*/) noexcept
{
  // using PringStrategy = ltu::FmtPrintStrategy;

  std::println("------------- LifetimeInfo example app -------------");

  constexpr auto wrapped_value = 5.8;
  std::println("wrapped_value = {}", wrapped_value);

  std::println("------------------------------");
  const ltu::LftLog ld1{ wrapped_value };
  std::println("li1 with wrapped_value = {}", ld1);

  std::println("------------------------------");
  constexpr auto wrapped_value2 = 42;
  const ltu::LftLog li2{ wrapped_value2 };
  std::println("li2 = {}", li2);

  std::println("------------------------------");

  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  std::println("{} + {} - {} = {}", ld1, li2, ltu::LftLog{ 2.73F }, ltu::LftLog{ ld1 + li2 - static_cast<double>(ltu::LftLog{ 2.73F }) });
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

  std::println("------------------------------");
  const ltu::LftLog<void> lv1;
  // Shouldn't compile due to static_assert in template specialisation for void in std::formatter<ltu::LftLog<...>>
  //  std::println("{}", lv1);


  return 0;
}
