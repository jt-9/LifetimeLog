#ifndef LTU_OBJECT_LIFETIME
#define LTU_OBJECT_LIFETIME


#include "StdPrint.hpp"
#include "StringifyTemplateArgs.hpp"

#include <concepts>
#include <stacktrace>
#include <string_view>
#include <thread>
#include <utility>

namespace ltu {
namespace {
#define LTU_TYPE_NAME_TO_STRING(A) \
  [[nodiscard]] static constexpr std::string_view type_to_string() noexcept { return std::string_view{ #A }; }

  template<typename Ptr>
    requires std::is_pointer_v<Ptr>
  [[nodiscard]] constexpr const void *cast_pointer_to_void(const Ptr ptr) noexcept
  {
    return static_cast<const void *>(ptr);
  }
}// namespace

template<typename T, class PrintStrategy> struct LftLog;

template<typename T,
  class PrintStrategy
#ifdef __cpp_lib_print
  = StdPrintStrategy
#endif
  >
struct LftLog : private PrintStrategy
{
  using value_type = T;

  constexpr LftLog() noexcept
    requires std::is_default_constructible_v<T>
  {
    PrintStrategy::println("{}() no argument constructor instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));
  }

  constexpr LftLog(const LftLog &src) noexcept
    requires std::is_copy_constructible_v<T>
    : t_{ src.t_ }
  {
    PrintStrategy::println("{}(const {} &src) copy constructor instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));
  }

  constexpr LftLog(LftLog &&src) noexcept
    requires std::is_move_constructible_v<T>
    : t_{ std::move(src.t_) }
  {
    PrintStrategy::println("{}({} &&src) move constructor instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));
  }

  // template<typename U>
  //   requires(!std::is_same_v<std::remove_cvref_t<U>, LftLog<T, PrintStrategy>>)
  // constexpr LftLog(U &&u) noexcept
  //   requires std::constructible_from<T, U>
  //   : t_{ std::forward<U>(u) }
  //{
  //   println("{}(U &&u) convert constructor instance {} thread {}",
  //     type_to_string(),
  //     cast_pointer_to_void(this),
  //     std::this_thread::get_id());
  // }

  explicit constexpr LftLog(const T &t) noexcept
    requires std::is_copy_constructible_v<T>
    : t_{ t }
  {
    PrintStrategy::println(
      "{}(const T &t) const l-value param constructor instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));
  }

  explicit constexpr LftLog(T &&t) noexcept
    requires std::is_move_constructible_v<T>
    : t_{ std::move(t) }
  {
    PrintStrategy::println("{}(T &&t) r-value param constructor instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));
  }

  constexpr ~LftLog() noexcept
  {
    PrintStrategy::println("~{}() destructor instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));
  }

  constexpr LftLog &operator=(const LftLog &rhs) & noexcept
    requires std::is_copy_assignable_v<T>
  {
    t_ = rhs.t_;

    PrintStrategy::println(
      "{}::operator=(const {} &rhs) copy assignment instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));

    return *this;
  }

  constexpr LftLog &operator=(LftLog &&rhs) noexcept
    requires std::is_move_assignable_v<T>
  {
    t_ = std::move(rhs.t_);

    PrintStrategy::println("{}::operator=({} &&rhs) move assignment instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));

    return *this;
  }

  [[nodiscard]] constexpr operator T &() & noexcept
  {
    PrintStrategy::println("{}::operator T&() cast operator instance {} thread {}\n\twith T = {}",
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));

    return t_;
  }

  [[nodiscard]] constexpr operator const T &() const & noexcept
  {
    PrintStrategy::println("{}::operator const T&() const cast operator instance {} thread {}\n\twith T = {}",
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));

    return t_;
  }

  [[nodiscard]] constexpr operator T &&() && noexcept
  {
    PrintStrategy::println("{}::operator T&&() cast operator instance {} thread {}\n\twith T = {}",
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));

    return std::move(t_);
  }

  [[nodiscard]] constexpr operator const T &&() const && noexcept
  {
    PrintStrategy::println("{}::operator const T&&() const cast operator instance {} thread {}\n\twith T = {}",
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));

    return std::move(t_);
  }

  T t_;

private:
  LTU_TYPE_NAME_TO_STRING(LftLog<T>)
};

template<class PrintStrategy> struct LftLog<void, PrintStrategy> : private PrintStrategy
{
  using value_type = void;

  constexpr LftLog() noexcept
  {
    PrintStrategy::println("{}() no argument constructor instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));
  }

  constexpr LftLog(const LftLog &) noexcept
  {
    PrintStrategy::println("{}(const {} &src) copy constructor instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));
  }

  constexpr LftLog(LftLog &&) noexcept
  {
    PrintStrategy::println("{}({} &&src) move constructor instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));
  }

  constexpr ~LftLog() noexcept
  {
    PrintStrategy::println("~{}() destructor instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));
  }

  constexpr LftLog &operator=(const LftLog &) & noexcept
  {
    PrintStrategy::println(
      "{}::operator=(const {} &rhs) copy assignment instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));

    return *this;
  }

  constexpr LftLog &operator=(LftLog &&) noexcept
  {
    PrintStrategy::println("{}::operator=({} &&rhs) move assignment instance {} thread {}\n\twith T = {}\n\tcaller {}",
      type_to_string(),
      type_to_string(),
      cast_pointer_to_void(this),
      std::this_thread::get_id(),
      GetTypeName<value_type>(),
      std::stacktrace::current(1, 1));

    return *this;
  }

private:
  LTU_TYPE_NAME_TO_STRING(LftLog<void>)
};

}// namespace ltu

#ifdef __cpp_lib_print
template<class CharT, typename Type, class PrintStrategy>
// requires(!std::is_void_v<Type>)
struct std::formatter<ltu::LftLog<Type, PrintStrategy>, CharT> : std::formatter<Type, CharT>
{
  static_assert(!std::is_void_v<Type>, "Type 'void' is not formattable");

  template<typename FormatContext> constexpr FormatContext::iterator format(const Type &value, FormatContext &ctx) const
  {
    return std::formatter<Type, CharT>::format(value, ctx);
  }
};
#endif

#endif// LTU_OBJECT_LIFETIME