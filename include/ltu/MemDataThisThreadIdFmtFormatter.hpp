#ifndef LTU_MEMBER_DATA_THIS_THREADID_FMT_FORMATTER
#define LTU_MEMBER_DATA_THIS_THREADID_FMT_FORMATTER

#include "FixedString.hpp"

#include <fmt/format.h>
#include <fmt/std.h>
#include <string_view>
#include <thread>
#include <type_traits>


namespace ltu {

template<fixed_string fmt = fixed_string{ "\n\tthis = {}, thread id = {},\n\twith T = {}" }>
struct MemDataThisThreadIdFmtFormatter final
{
  [[nodiscard]] static auto format(const void *const ptr, std::string_view arg_type_name) noexcept
  {
    return fmt::format(fmt, ptr, std::this_thread::get_id(), arg_type_name);
  }
};

}// namespace ltu

#endif// LTU_MEMBER_DATA_THIS_THREADID_FMT_FORMATTER