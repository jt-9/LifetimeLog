#ifndef LTU_MEMBER_DATA_TYPE_THIS_FMT_FORMATTER
#define LTU_MEMBER_DATA_TYPE_THIS_FMT_FORMATTER

#include "FixedString.hpp"

#include <fmt/format.h>
#include <string_view>
#include <type_traits>


namespace ltu {

template<fixed_string fmt = "\n\tthis = {}\n\twith T = {}">
struct MemDataThisFmtFormatter final
{
  [[nodiscard]] static auto format(const void *const ptr, std::string_view arg_type_name) noexcept
  {
    return fmt::format(fmt, ptr, arg_type_name);
  }
};

}// namespace ltu

#endif// LTU_MEMBER_DATA_TYPE_THIS_FMT_FORMATTER