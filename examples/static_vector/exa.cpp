#include <atomic>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <vector>

template <typename T, std::size_t N> class static_vector
{
  static_assert(std::is_destructible_v<T>);
  static_assert(N > 0);

public:
  using payload_t = std::remove_cv_t<T>;

private:
  using mem_payload_t                            = std::aligned_storage_t<sizeof(payload_t), alignof(payload_t)>;
  static constexpr std::size_t number_of_entries = N;

  static constexpr bool is_copy_constructible_v         = std::is_copy_constructible_v<payload_t>;
  static constexpr bool is_nothrow_copy_constructible_v = std::is_nothrow_copy_constructible_v<payload_t>;
  static constexpr bool is_move_constructible_v         = std::is_move_constructible_v<payload_t>;
  static constexpr bool is_nothrow_move_constructible_v = std::is_nothrow_move_constructible_v<payload_t>;
  static constexpr bool is_nothrow_destructible_v       = std::is_nothrow_destructible_v<payload_t>;

public:
  inline constexpr static_vector() noexcept = default;
  inline ~static_vector() noexcept(is_nothrow_destructible_v)
  {
    for (std::size_t idx = 0; idx < this->m_idx; idx)
    {
      payload_t& tmp = *std::launder(reinterpret_cast<payload_t*>(&this->m_mem[idx]));
      tmp.~payload_t();
    }
  }

  template <typename = std::enable_if_t<is_copy_constructible_v>> constexpr bool push_back(payload_t const& v) noexcept(is_nothrow_copy_constructible_v)
  {
    if (this->m_idx >= number_of_entries)
      return false;

    ::new (&this->m_mem[this->m_idx++]) payload_t(v);
    return true;
  }

  template <typename = std::enable_if_t<is_move_constructible_v>> constexpr bool push_back(payload_t&& v) noexcept(is_nothrow_move_constructible_v)
  {
    if (this->m_idx >= number_of_entries)
      return false;

    ::new (&this->m_mem[this->m_idx++]) payload_t(std::move(v));
    return true;
  }

  template <typename... Ts, typename = std::enable_if_t<std::is_constructible_v<payload_t, Ts...>>>
  constexpr bool emplace_back(Ts&&... args) noexcept(std::is_nothrow_constructible_v<payload_t, Ts...>)
  {
    if (this->m_idx >= number_of_entries)
      return false;

    ::new (&this->m_mem[this->m_idx++]) payload_t(args...);
    return true;
  }

  constexpr std::size_t get_number_of_entries() const noexcept { return number_of_entries; }

  constexpr std::size_t get_number_of_used_entries() const noexcept { return this->m_idx; }

  constexpr std::size_t get_number_of_free_entries() const noexcept { return number_of_entries - this->m_idx; }

  constexpr payload_t& operator[](std::size_t const& idx) & { return *std::launder(reinterpret_cast<payload_t*>(&this->m_mem[idx])); }

  constexpr payload_t const& operator[](std::size_t const& idx) const& { return *std::launder(reinterpret_cast<payload_t const*>(&this->m_mem[idx])); }

private:
  mem_payload_t m_mem[number_of_entries]{};
  std::size_t   m_idx = 0;
};

#include <iostream>

int main()
{
  static_vector<int, 10> buffer;

  {
    int tmp = 3;
    buffer.push_back(tmp);
  }
  buffer.push_back(1);
  buffer.emplace_back(2);

  std::cout << buffer[0] << std::endl;

  return 0;
}
