#ifndef LTU_OBJECT_LIFETIME
#define LTU_OBJECT_LIFETIME


#include "MemDataThisStdFormatter.hpp"
#include "StdPrint.hpp"
#include "StringifyTemplateArgs.hpp"

#include <concepts>
// #include <stacktrace>
#include <string_view>
#include <type_traits>
#include <utility>


namespace ltu {
namespace {
#define LTU_TYPE_NAME_TO_STRING(A) \
  [[nodiscard]] static constexpr std::string_view type_to_string() noexcept { return std::string_view{ #A }; }

  template<typename Ptr>
    requires std::is_pointer_v<Ptr>
  [[nodiscard]] constexpr const void *cast_pointer_to_void(const Ptr ptr) noexcept
  { return static_cast<const void *>(ptr); }
}// namespace

template<typename T, class PrintStrategy, class MemDataFormatter> struct LftLog;

template<typename T,
  class PrintStrategy
#ifdef __cpp_lib_print
  = StdPrintStrategy
#endif
  ,
  class MemDataFormatter = MemDataThisStdFormatter<>>
struct LftLog
{
  using value_type = T;
  static constexpr auto k_arg_type_name = GetTypeName<value_type>();

  constexpr LftLog() noexcept(std::is_nothrow_default_constructible_v<T>)
    requires std::is_default_constructible_v<T>
  {
    PrintStrategy::println(
      "{}() no argument ctor{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));
  }

  constexpr LftLog(const LftLog &src) noexcept(std::is_nothrow_copy_constructible_v<T>)
    requires std::is_copy_constructible_v<T>
    : t_{ src.t_ }
  {
    PrintStrategy::println(
      "{0}(const {0} &src) copy ctor{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));
  }

  constexpr LftLog(LftLog &&src) noexcept(std::is_nothrow_move_constructible_v<T>)
    requires std::is_move_constructible_v<T>
    : t_{ std::move(src.t_) }
  {
    PrintStrategy::println(
      "{0}({0} &&src) move ctor{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));
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

  explicit constexpr LftLog(const T &t) noexcept(std::is_nothrow_copy_constructible_v<T>)
    requires std::is_copy_constructible_v<T>
    : t_{ t }
  {
    PrintStrategy::println(
      "{}(const T &t) const l-value param ctor{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));
  }

  explicit constexpr LftLog(T &&t) noexcept(std::is_nothrow_move_constructible_v<T>)
    requires std::is_move_constructible_v<T>
    : t_{ std::move(t) }
  {
    PrintStrategy::println(
      "{}(T &&t) r-value param ctor{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));
  }

  template<class... Args>
    requires std::is_constructible_v<T, Args...>
  explicit constexpr LftLog(std::in_place_t, Args &&...args) noexcept(std::is_nothrow_constructible_v<T, Args...>) 
    : t_{ std::forward<Args>(args)... }
  {
    PrintStrategy::println(
      "{}(std::in_place_t, Args... args) in place ctor{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));
  }

  template<class U, class... Args>
    requires std::is_constructible_v<T, std::initializer_list<U>&, Args...>
  explicit constexpr LftLog(std::in_place_t, std::initializer_list<U> ilist, Args &&...args) noexcept(
    std::is_nothrow_constructible_v<T, std::initializer_list<U> &, Args...>) 
    : t_{ ilist, std::forward<Args>(args)... }
  {
    PrintStrategy::println("{}(std::in_place_t, std::initializer_list<U> ilist, Args... args) in place ctor{}",
      type_to_string(),
      MemDataFormatter::format(this, k_arg_type_name));
  }

  constexpr ~LftLog() noexcept(std::is_nothrow_destructible_v<T>)
  { PrintStrategy::println("~{}() dtor{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name)); }

  constexpr LftLog &operator=(const LftLog &rhs) & noexcept(std::is_nothrow_assignable_v<T>)
    requires std::is_copy_assignable_v<T>
  {
    t_ = rhs.t_;

    PrintStrategy::println("{0}::operator=(const {0} &rhs) copy assign{}",
      type_to_string(),
      MemDataFormatter::format(this, k_arg_type_name));

    return *this;
  }

  constexpr LftLog &operator=(LftLog &&rhs) noexcept(std::is_nothrow_move_assignable_v<T>)
    requires std::is_move_assignable_v<T>
  {
    t_ = std::move(rhs.t_);

    PrintStrategy::println(
      "{0}::operator=({0} &&rhs) move assign{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));

    return *this;
  }

  [[nodiscard]] constexpr operator T &() & noexcept
  {
    PrintStrategy::println("{}::operator T&(){}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));

    return t_;
  }

  [[nodiscard]] constexpr operator const T &() const & noexcept
  {
    PrintStrategy::println(
      "{}::operator const T&(){}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));

    return t_;
  }

  [[nodiscard]] constexpr operator T &&() && noexcept
  {
    PrintStrategy::println("{}::operator T&&(){}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));

    return std::move(t_);
  }

  [[nodiscard]] constexpr operator const T &&() const && noexcept
  {
    PrintStrategy::println(
      "{}::operator const T&&() const{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));

    return std::move(t_);
  }
  // cppcheck-suppress uninitMemberVarNoCtor; Intended
  T t_;

private:
  LTU_TYPE_NAME_TO_STRING(LftLog<T>)
};

template<class PrintStrategy, class MemDataFormatter> struct LftLog<void, PrintStrategy, MemDataFormatter>
{
  using value_type = void;
  static constexpr auto k_arg_type_name = GetTypeName<value_type>();

  constexpr LftLog() noexcept
  {
    PrintStrategy::println(
      "{}() no argument ctor{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));
  }

  constexpr LftLog(const LftLog &) noexcept
  {
    PrintStrategy::println(
      "{0}(const {0} &src) copy ctor{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));
  }

  constexpr LftLog(LftLog &&) noexcept
  {
    PrintStrategy::println(
      "{0}({0} &&src) move ctor{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));
  }

  constexpr ~LftLog() noexcept
  { PrintStrategy::println("~{}() dtor{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name)); }

  constexpr LftLog &operator=(const LftLog &) & noexcept
  {
    PrintStrategy::println("{0}::operator=(const {0} &rhs) copy assign{}",
      type_to_string(),
      MemDataFormatter::format(this, k_arg_type_name));

    return *this;
  }

  constexpr LftLog &operator=(LftLog &&) noexcept
  {
    PrintStrategy::println(
      "{0}::operator=({0} &&rhs) move assign{}", type_to_string(), MemDataFormatter::format(this, k_arg_type_name));

    return *this;
  }

private:
  LTU_TYPE_NAME_TO_STRING(LftLog<void>)
};

}// namespace ltu

#ifdef __cpp_lib_print
template<class CharT, typename Type, class PrintStrategy, class MemDataFormatter>
  requires std::formattable<Type, CharT>
struct std::formatter<ltu::LftLog<Type, PrintStrategy, MemDataFormatter>, CharT>
  : std::formatter<std::remove_cvref_t<Type>, CharT>
{
  static_assert(!std::is_void_v<Type>, "Type 'void' is not formattable");

private:
  using maybe_const_LftLog = std::conditional_t<std::formattable<const Type, CharT>,
    const ltu::LftLog<Type, PrintStrategy, MemDataFormatter>,
    ltu::LftLog<Type, PrintStrategy, MemDataFormatter>>;

public:
  template<typename FormatContext>
  constexpr FormatContext::iterator format(maybe_const_LftLog &value, FormatContext &ctx) const
  { return std::formatter<std::remove_cvref_t<Type>, CharT>::format(value.t_, ctx); }
};
#endif

#endif// LTU_OBJECT_LIFETIME