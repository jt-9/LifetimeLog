#ifndef LTU_CALL_LOGGER
#define LTU_CALL_LOGGER


#include "MemDataThisStdFormatter.hpp"
#include "StdPrint.hpp"
#include "StringifyTemplateArgs.hpp"

#include <concepts>
// #include <stacktrace>
#include <iostream>
#include <source_location>
#include <string>
#include <type_traits>

namespace ltu {

// Logs to cout, with current indent
#define LOG(...) std::cout << std::string(ScopedLogger::indent, ' ') << __VA_ARGS__ << std::endl

// Logs in and out of a scope
struct ScopedLogger
{
  static thread_local int indent;
  std::string_view m_name;

  explicit constexpr ScopedLogger(std::string_view name) noexcept
  : m_name{name}
  {
    LOG("\\" << m_name);
    indent++;
  }

  constexpr ~ScopedLogger() noexcept
  {
    indent--;
    LOG("/" << m_name);
  }
};

int ScopedLogger::indent = 0;// Should go in cpp file, if we have one

// Creates a ScopedLogger for the current function
#define LOGF() \
  ScopedLogger logger##__LINE__ { std::source_location::current().function_name() }

}// namespace ltu

#endif// LTU_CALL_LOGGER