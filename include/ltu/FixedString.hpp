#ifndef LTU_FIXED_STRING
#define LTU_FIXED_STRING

#include <cstddef>// std::size_t
#include <stdexcept>// std::out_of_range
#include <string_view>
// #include <initializer_list>
#include <iterator>
#include <limits>

namespace ltu {
// forward declaration
template<class charT, std::size_t N> class basic_fixed_string;


// Alias templates fixed_string, u16fixed_string, u32fixed_string,
// wfixed_string.
template<size_t N> using fixed_string = basic_fixed_string<char, N>;
template<size_t N> using u16fixed_string = basic_fixed_string<char16_t, N>;
template<size_t N> using u32fixed_string = basic_fixed_string<char32_t, N>;
template<size_t N> using wfixed_string = basic_fixed_string<wchar_t, N>;

// Creates a fixed_string from a string literal.
template<class charT, size_t N1>
[[nodiscard]] constexpr basic_fixed_string<charT, N1 - 1> make_fixed_string(const charT (&a)[N1]) noexcept
{
  return basic_fixed_string<charT, N1 - 1>{ a };
}

template<class charT, std::size_t N> class basic_fixed_string
{
public:
  typedef charT value_type;

  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;

  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  typedef std::basic_string_view<charT> view;

  static constexpr auto npos = view::npos;

  // Implicit conversion to string_view
  [[nodiscard]] constexpr operator view() const noexcept { return { data_, N }; }

  // Default construct to all zeros.
  constexpr basic_fixed_string() noexcept : data_{ 0 }
  {
    // for (std::size_t i = 0; i < N + 1; i++) data_[i] = 0;
  }

  // Copy constructor.
  constexpr basic_fixed_string(const basic_fixed_string &str) noexcept//: data_{ 0 }
  {
    for (std::size_t i = 0; i < N + 1; i++) data_[i] = str[i];
  }

  // Converting constructor from string literal.
  explicit constexpr basic_fixed_string(const charT (&arr)[N + 1]) noexcept// : data_{ 0 }
  {
    for (std::size_t i = 0; i < N + 1; i++) data_[i] = arr[i];
  }

  // Copy assignment.
  [[nodiscard]] constexpr basic_fixed_string &operator=(const basic_fixed_string &str) noexcept
  {
    for (std::size_t i = 0; i < N + 1; i++) data_[i] = str[i];
  }

  // Assign from string literal.
  [[nodiscard]] constexpr basic_fixed_string &operator=(const charT (&arr)[N + 1]) noexcept
  {
    for (std::size_t i = 0; i < N + 1; i++) data_[i] = arr[i];
  }

  // c/r/begin, c/r/end.
  [[nodiscard]] constexpr iterator begin() noexcept { return data_; }
  [[nodiscard]] constexpr const_iterator begin() const noexcept { return data_; }
  [[nodiscard]] constexpr iterator end() noexcept { return data_ + N; }
  [[nodiscard]] constexpr const_iterator end() const noexcept { return data_ + N; }
  [[nodiscard]] constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
  [[nodiscard]] constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
  [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return data_; }
  [[nodiscard]] constexpr const_iterator cend() const noexcept { return data_ + N; }
  [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
  [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

  // size, empty, length.
  [[nodiscard]] static constexpr std::size_t size() noexcept { return N; }
  [[nodiscard]] static constexpr bool empty() noexcept { return N == 0; }
  [[nodiscard]] static constexpr std::size_t length() noexcept { return N; }

  // str[pos]
  [[nodiscard]] constexpr reference operator[](std::size_t pos) noexcept { return data_[pos]; }
  [[nodiscard]] constexpr const_reference operator[](std::size_t pos) const noexcept { return data_[pos]; }

  // str.at(pos)
  [[nodiscard]] constexpr reference at(std::size_t pos)
  {
    if (pos >= N) throw std::out_of_range("invalid fixed_string position");
    return data_[pos];
  }

  [[nodiscard]] constexpr const_reference at(std::size_t pos) const
  {
    if (pos >= N) throw std::out_of_range("invalid fixed_string position");
    return data_[pos];
  }

  // front, back.
  [[nodiscard]] constexpr const_reference front() const noexcept { return data_[0]; }
  [[nodiscard]] constexpr reference front() { return data_[0]; }
  [[nodiscard]] constexpr const_reference back() const noexcept { return data_[N - 1]; }
  [[nodiscard]] constexpr reference back() noexcept { return data_[N - 1]; }

private:
  [[nodiscard]] static constexpr std::size_t helper_substr_length(std::size_t pos, std::size_t count)
  {
    if (pos >= N)
      return 0;
    else if (count == npos || pos + count > N)
      return N - pos;
    else
      return count;
  }

public:
  // str.substr<pos,count>()
  template<std::size_t pos = 0, std::size_t count = npos>
  [[nodiscard]] constexpr basic_fixed_string<charT, helper_substr_length(pos, count)> substr() const noexcept
  {
    constexpr std::size_t n = helper_substr_length(pos, count);

    basic_fixed_string<charT, n> result;
    for (std::size_t i = 0; i < n; i++) result[i] = data_[pos + i];
    return result;
  }

  // str1.assign(str2).  Must be equal size.
  [[nodiscard]] constexpr basic_fixed_string &assign(view str)
  {
    if (str.size() != N) { throw std::invalid_argument("str.size() and *this.size() must be equal"); }

    for (std::size_t i = 0; i < N; i++) data_[i] = str[i];
  }

  // Replace substring.
  [[nodiscard]] constexpr basic_fixed_string &replace(std::size_t pos, view str)
  {
    if (pos + str.size() > N) throw std::invalid_argument("pos + str.size() must be greater than *this.size()");
    for (std::size_t i = 0; i < str.size(); i++) data_[i] = pos;
    return *this;
  }

  // Swap with fixed_string of equal size.
  constexpr void swap(basic_fixed_string &str)
  {
    using std::swap;
    for (std::size_t i = 0; i < N; i++) swap(data_[i], str[i]);
  }

  // Null-terminated C string.
  [[nodiscard]] constexpr const charT *c_str() const noexcept { return data_; }
  [[nodiscard]] constexpr const charT *data() const noexcept { return data_; }

  [[nodiscard]] constexpr int compare(view str) const { return view(*this).compare(str); }

  [[nodiscard]] constexpr int compare(std::size_t pos1, std::size_t n1, view str) const
  {
    return view(*this).compare(pos1, n1, str);
  }

  [[nodiscard]] constexpr int
    compare(std::size_t pos1, std::size_t n1, view str, std::size_t pos2, std::size_t n2 = npos) const
  {
    return view(*this).compare(pos1, n1, str, pos2, n2);
  }

  [[nodiscard]] constexpr int compare(const charT *s) const { return view(*this).compare(s); }

  [[nodiscard]] constexpr int compare(std::size_t pos1, std::size_t n1, const charT *s) const
  {
    return view(*this).compare(pos1, n1, s);
  }

  [[nodiscard]] constexpr int compare(std::size_t pos1, std::size_t n1, const charT *s, std::size_t n2) const
  {
    return view(*this).compare(pos1, n1, s, n2);
  }

  [[nodiscard]] constexpr std::size_t find(view str, std::size_t pos = 0) const noexcept
  {
    return view(*this).find(str, pos);
  }

  [[nodiscard]] constexpr std::size_t find(charT c, std::size_t pos = 0) const noexcept
  {
    return view(*this).find(c, pos);
  }

  [[nodiscard]] constexpr std::size_t find(const charT *s, std::size_t pos, std::size_t count) const
  {
    return view(*this).find(s, pos, count);
  }

  [[nodiscard]] constexpr std::size_t find(const charT *s, std::size_t pos = 0) const
  {
    return view(*this).find(s, pos);
  }

  template<std::size_t M> [[nodiscard]] constexpr std::size_t rfind(view str, std::size_t pos = npos) const noexcept
  {
    return view(*this).rfind(str, pos);
  }

  [[nodiscard]] constexpr std::size_t rfind(const charT *s, std::size_t pos, std::size_t n) const
  {
    return view(*this).rfind(s, pos, n);
  }

  [[nodiscard]] constexpr std::size_t rfind(const charT *s, std::size_t pos = npos) const
  {
    return view(*this).rfind(s, pos);
  }

  [[nodiscard]] constexpr std::size_t rfind(charT c, std::size_t pos = npos) const { return view(*this).rfind(c, pos); }

  [[nodiscard]] constexpr std::size_t find_first_of(view str, std::size_t pos = 0) const
  {
    return view(*this).find_first_of(str, pos);
  }

  [[nodiscard]] constexpr std::size_t find_first_of(const charT *s, std::size_t pos, std::size_t n) const
  {
    return view(*this).find_first_of(s, pos, n);
  }

  [[nodiscard]] constexpr std::size_t find_first_of(const charT *s, std::size_t pos = 0) const
  {
    return view(*this).find_first_of(s, pos);
  }

  [[nodiscard]] constexpr std::size_t find_first_of(charT c, std::size_t pos = 0) const
  {
    return view(*this).find_first_of(c, pos);
  }

  template<std::size_t M> [[nodiscard]] constexpr std::size_t find_last_of(view str, std::size_t pos = npos) const
  {
    return view(*this).find_last_of(str, pos);
  }

  [[nodiscard]] constexpr std::size_t find_last_of(const charT *s, std::size_t pos, std::size_t n) const
  {
    return view(*this).find_last_of(s, pos, n);
  }

  [[nodiscard]] constexpr std::size_t find_last_of(const charT *s, std::size_t pos = npos) const
  {
    return view(*this).find_last_of(s, pos);
  }

  [[nodiscard]] constexpr std::size_t find_last_of(charT c, std::size_t pos = npos) const
  {
    return view(*this).find_last_of(c, pos);
  }

  template<std::size_t M>
  [[nodiscard]] constexpr std::size_t find_first_not_of(view str, std::size_t pos = 0) const noexcept
  {
    return view(*this).find_first_not_of(str, pos);
  }

  [[nodiscard]] constexpr std::size_t find_first_not_of(const charT *s, std::size_t pos, std::size_t n) const
  {
    return view(*this).find_first_not_of(s, pos, n);
  }

  [[nodiscard]] constexpr std::size_t find_first_not_of(const charT *s, std::size_t pos = 0) const
  {
    return view(*this).find_first_not_of(s, pos);
  }

  [[nodiscard]] constexpr std::size_t find_first_not_of(charT c, std::size_t pos = 0) const
  {
    return view(*this).find_first_not_of(c, pos);
  }

  [[nodiscard]] constexpr std::size_t find_last_not_of(view str, std::size_t pos = npos) const noexcept
  {
    return view(*this).find_last_not_of(str, pos);
  }

  [[nodiscard]] constexpr std::size_t find_last_not_of(const charT *s, std::size_t pos, std::size_t n) const
  {
    return view(*this).find_last_not_of(s, pos, n);
  }

  [[nodiscard]] constexpr std::size_t find_last_not_of(const charT *s, std::size_t pos = npos) const
  {
    return view(*this).find_last_not_of(s, pos);
  }

  [[nodiscard]] constexpr std::size_t find_last_not_of(charT c, std::size_t pos = npos) const
  {
    return view(*this).find_last_not_of(c, pos);
  }

public:
  charT data_[N + 1];// exposition only
                     // (+1 is for terminating null)
};

// Deduction guide for basic_fixed_string
template<class charT, std::size_t N> basic_fixed_string(const charT (&)[N]) -> basic_fixed_string<charT, N - 1>;

}// namespace ltu

#endif// LTU_FIXED_STRING