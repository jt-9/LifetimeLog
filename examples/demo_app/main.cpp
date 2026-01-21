#include "ltu/LftLog.hpp"

//*/
#include "ltu/StdPrint.hpp"
#include "ltu/MemDataThisThreadIdStdFormatter.hpp"

#include <print>

namespace proj_fmt = ::std;
using LtuPrintStrategy = ltu::StdPrintStrategy;
using LtuMemDataFormatter = ltu::MemDataThisThreadIdStdFormatter;
//*/
/*/
#include "ltu/FmtFormatter.hpp"
#include "ltu/FmtPrint.hpp"
#include "ltu/MemDataThisThreadIdFmtFormatter.hpp"

#include <fmt/base.h>

namespace proj_fmt = ::fmt;
using LtuPrintStrategy = ltu::FmtPrintStrategy;
using LtuMemDataFormatter = ltu::MemDataThisThreadIdFmtFormatter;
//*/


// NOLINTNEXTLINE(bugprone-exception-escape)
int main(/*int argc, char **argv*/) noexcept
{
  proj_fmt::println("------------- LifetimeInfo example app -------------");

  constexpr auto wrapped_value = 5.8;
  proj_fmt::println("wrapped_value = {}", wrapped_value);

  proj_fmt::println("------------------------------");
  const ltu::LftLog<decltype(wrapped_value), LtuPrintStrategy, LtuMemDataFormatter> ld1{ wrapped_value };
  proj_fmt::println("li1 with wrapped_value = {}", ld1);

  proj_fmt::println("------------------------------");
  constexpr auto wrapped_value2 = 42;
  const ltu::LftLog<decltype(wrapped_value2), LtuPrintStrategy, LtuMemDataFormatter> li2{ wrapped_value2 };
  proj_fmt::println("li2 = {}", li2);

  proj_fmt::println("------------------------------");

  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  proj_fmt::println("{} + {} - {} = {}",
    ld1,
    li2,
    ltu::LftLog<const float, LtuPrintStrategy, LtuMemDataFormatter>{ 2.73F },
    ltu::LftLog<const double, LtuPrintStrategy, LtuMemDataFormatter>{
      ld1 + li2 - static_cast<double>(ltu::LftLog<const float, LtuPrintStrategy, LtuMemDataFormatter>{ 2.73F }) });
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

  proj_fmt::println("------------------------------");
  const ltu::LftLog<void, LtuPrintStrategy, LtuMemDataFormatter> lv1;
  // Shouldn't compile due to static_assert in template specialisation for void in std::formatter<ltu::LftLog<...>>
  //  std::println("{}", lv1);


  return 0;
}
