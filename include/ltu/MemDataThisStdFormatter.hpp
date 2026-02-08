#ifndef LTU_MEMBER_DATA_TYPE_THIS_STD_FORMATTER
#define LTU_MEMBER_DATA_TYPE_THIS_STD_FORMATTER

#include "FixedString.hpp"

#include <format>
#include <string_view>
#include <type_traits>


namespace ltu {

template<fixed_string fmt = fixed_string{ "\n\tthis = {}\n\twith T = {}" }>
struct MemDataThisStdFormatter final
{
  [[nodiscard]] static auto format(const void *const ptr, std::string_view arg_type_name) noexcept
  {
    return std::format(fmt, ptr, arg_type_name);
  }
};

}// namespace ltu

#endif// LTU_MEMBER_DATA_TYPE_THIS_STD_FORMATTER