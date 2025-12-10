// #include "ltu/FmtFormatter.hpp"
// #include "ltu/FmtPrint.hpp"
// #include "ltu/LftLog.hpp"

// #include <print>
#include <fmt/base.h>


int main(/*int argc, char **argv*/) noexcept
{
  // using PringStrategy = ltu::FmtPrintStrategy;

  fmt::println("------------- LifetimeInfo example app -------------");

  constexpr auto wrapped_value = 5.8;
  fmt::println("wrapped_value = {}", wrapped_value);

  // ltu::LftLog<double, ltu::FmtPrintStrategy> li1{ wrapped_value };
  // fmt::println("li1 with wrapped_value = {}", li1);

  // const auto wrapped_value2 = 42;
  // const ltu::LftLog<int, ltu::FmtPrintStrategy> li2{ wrapped_value2 };
  // fmt::println("li2 = {}", li2);

  // fmt::println("{} + {} = {}", li1, li2, ltu::LftLog{li1 + li2});

  // ltu::LftLog<void, ltu::FmtPrintStrategy> lv1;
  //  std::println("{}", lv1);

  // constexpr auto wrapped_value2 = -42;
  // ltu::LftLog lli1{ ltu::LftLog{ wrapped_value2 } };
  // fmt::println("LftLog<LftLog> lli = {}", lli1);

  return 0;
}
