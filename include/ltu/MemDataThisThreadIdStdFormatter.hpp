#ifndef LTU_MEMBER_DATA_TYPE_THIS_THREADID_STD_FORMATTER
#define LTU_MEMBER_DATA_TYPE_THIS_THREADID_STD_FORMATTER

#include <format>
#include <string_view>
#include <thread>
#include <type_traits>


namespace ltu {

struct MemDataThisThreadIdStdFormatter final
{
  template<typename Pointer>
    requires std::is_pointer_v<Pointer>
  [[nodiscard]] static auto format(const Pointer ptr, std::string_view arg_type_name) noexcept
  {
    return std::format("this = {}, thread id = {},\n\twith T = {}",
      static_cast<const void *>(ptr),
      std::this_thread::get_id(),
      arg_type_name);
  }
};

}// namespace ltu

#endif// LTU_MEMBER_DATA_TYPE_THIS_THREADID_STD_FORMATTER