#ifndef LTU_MEMBER_DATA_TYPE_THIS_THREADID_STD_FORMATTER
#define LTU_MEMBER_DATA_TYPE_THIS_THREADID_STD_FORMATTER

#include "FixedString.hpp"

#include <format>
#include <string_view>
#include <thread>
#include <type_traits>


namespace ltu {

template<fixed_string fmt = "\n\tthis = {}, thread id = {},\n\twith T = {}">
struct MemDataThisThreadIdStdFormatter final
{
  [[nodiscard]] static auto format(const void *const ptr, std::string_view arg_type_name) noexcept
  {
    return std::format(fmt.view(), ptr, std::this_thread::get_id(), arg_type_name);
  }
};

}// namespace ltu

#endif// LTU_MEMBER_DATA_TYPE_THIS_THREADID_STD_FORMATTER