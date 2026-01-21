#include <algorithm>
#include <cstddef>
#include <ranges>
#include <string_view>


namespace ltu {

namespace details {

  [[nodiscard]]
  constexpr bool is_space(char p, ::std::string_view const whitespaces) noexcept
  {
    auto ne = [p](auto q) { return p != q; };
    return !!(whitespaces | std::views::drop_while(ne));
  };

  [[nodiscard]]
  constexpr std::string_view trim(::std::string_view const in) noexcept
  {
    using namespace std::literals::string_view_literals;

    //" \t\n\v\r\f"sv
    /*static*/ constexpr auto kWhitespaces = " \t"sv;

    std::size_t start = 0;
    while (start < in.size() && is_space(in[start], kWhitespaces)) { ++start; }

    std::size_t end = in.size();
    while (end > 0 && is_space(in[end - 1], kWhitespaces)) { --end; }

    return in.substr(start, end - start);
  }

  [[nodiscard]] consteval ::std::string_view
    GetTypeNameSubStr(::std::string_view kFullName, ::std::string_view front, ::std::string_view back) noexcept
  {
    const std::size_t kStartPos = kFullName.find(front);
    std::size_t ArgStartPos = (kStartPos != std::string_view::npos) ? kStartPos + front.size() : 0;

    const std::size_t kEndPos = kFullName.find(back);
    std::size_t ArgEndPos = (kEndPos != std::string_view::npos) ? kEndPos : kFullName.size();

    return trim(kFullName.substr(ArgStartPos, ArgEndPos - ArgStartPos));
  }

#if (_MSC_VER) && (!__INTEL_COMPILER)

  template<typename T> struct GetTypeNameHelper
  {
    [[nodiscard]] static constexpr ::std::string_view GetTypeName(void) noexcept
    {
      using namespace std::literals::string_view_literals;
      return GetTypeNameSubStr(::std::string_view{ __FUNCTION__ }, "GetTypeNameHelper<"sv, ">::GetTypeName"sv);
    }
  };

#elif __clang__

  template<typename T> struct GetTypeNameHelper
  {
    [[nodiscard]] static constexpr ::std::string_view GetTypeName(void) noexcept
    {
      using namespace std::literals::string_view_literals;
      return GetTypeNameSubStr(::std::string_view{ __PRETTY_FUNCTION__ }, "GetTypeNameHelper<"sv, ">::GetTypeName"sv);
    }
  };

#elif __GNUC__

  template<typename T> struct GetTypeNameHelper
  {
    [[nodiscard]] static constexpr ::std::string_view GetTypeName(void) noexcept
    {
      using namespace std::literals::string_view_literals;
      return GetTypeNameSubStr(::std::string_view{ __PRETTY_FUNCTION__ }, "with T = "sv, ";"sv);
    }
  };

#endif
}// namespace details

template<typename T> [[nodiscard]] constexpr ::std::string_view GetTypeName(void) noexcept
{
  return details::GetTypeNameHelper<T>::GetTypeName();
}

}// namespace ltu
