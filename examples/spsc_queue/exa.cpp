#include <atomic>
#include <cstdint>
#include <optional>
#include <type_traits>

template <typename T, std::size_t N> class spsc
{
  static_assert(std::is_destructible_v<T>);
  static_assert(std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>);
  static_assert(N > 0);

public:
  using payload_t = std::remove_cv_t<T>;

private:
  using mem_payload_t                            = std::aligned_storage_t<sizeof(payload_t), alignof(payload_t)>;
  static constexpr std::size_t number_of_entries = N;
  static constexpr std::size_t max_idx           = number_of_entries + 1;

  static constexpr bool is_copy_constructible_v         = std::is_copy_constructible_v<payload_t>;
  static constexpr bool is_nothrow_copy_constructible_v = std::is_nothrow_copy_constructible_v<payload_t>;
  static constexpr bool is_move_constructible_v         = std::is_move_constructible_v<payload_t>;
  static constexpr bool is_nothrow_move_constructible_v = std::is_nothrow_move_constructible_v<payload_t>;
  static constexpr bool is_nothrow_destructible_v       = std::is_nothrow_destructible_v<payload_t>;

public:
  inline constexpr spsc() noexcept = default;

  inline ~spsc() noexcept(is_nothrow_destructible_v)
  {
    std::size_t const w = this->m_w_idx;
    std::size_t       r = this->m_r_idx;
    while (r != w)
    {
      payload_t& tmp = *std::launder(reinterpret_cast<payload_t*>(&this->m_mem[r]));
      tmp.~payload_t();
      r = advance(r, 1);
    }
    this->m_r_idx = r;
  }

  template <typename = std::enable_if_t<is_copy_constructible_v>> constexpr bool push_back(payload_t const& v) noexcept(is_nothrow_copy_constructible_v)
  {
    std::size_t const r      = this->m_r_idx;
    std::size_t const w      = this->m_w_idx;
    std::size_t const w_next = advance(w, 1);

    if (w_next == r)
      return false;

    ::new (&this->m_mem[w]) payload_t(v);

    this->m_w_idx = w_next;
    return true;
  }

  template <typename = std::enable_if_t<is_move_constructible_v>>constexpr bool push_back(payload_t&& v) noexcept(is_nothrow_move_constructible_v)
  {
    std::size_t const r      = this->m_r_idx;
    std::size_t const w      = this->m_w_idx;
    std::size_t const w_next = advance(w, 1);

    if (w_next == r)
      return false;

    ::new (&this->m_mem[w]) payload_t(std::move(v));

    this->m_w_idx = w_next;
    return true;
  }

  constexpr std::optional<payload_t> pop_front() noexcept(is_move_constructible_v ? is_nothrow_move_constructible_v : is_nothrow_copy_constructible_v)
  {
    std::size_t const w = this->m_w_idx;
    std::size_t const r = this->m_r_idx;

    if (r == w)
      return std::nullopt;

    payload_deleter_t tmp{ *std::launder(reinterpret_cast<payload_t*>(&this->m_mem[r])), this->m_r_idx, advance(r, 1) };
    if constexpr (is_move_constructible_v)
    {
      return { std::move(tmp.obj) };
    }
    else
    {
      return { tmp.obj };
    }
  }

  constexpr std::size_t get_number_of_entries() const noexcept { return number_of_entries; }

  constexpr std::size_t get_number_of_used_entries() const noexcept
  {
    std::size_t const r = this->m_r_idx;
    std::size_t const w = this->m_w_idx;

    if (r <= w)
      return w - r;
    return max_idx + w - r;
  }

  constexpr std::size_t get_number_of_free_entries() const noexcept
  {
    std::size_t const r = this->m_r_idx;
    std::size_t const w = this->m_w_idx;

    if (r <= w)
      return number_of_entries + r - w;
    return r - w - 1;
  }

private:
  static inline constexpr std::size_t advance(std::size_t idx, std::size_t val) noexcept
  {
    std::size_t ret = idx + val;
    while (ret >= max_idx)
    {
      ret -= max_idx;
    }
    return ret;
  }

  struct payload_deleter_t
  {
  public:
    inline constexpr payload_deleter_t(payload_t& obj, std::atomic<std::size_t>& r_idx, std::size_t next_idx_value) noexcept
        : obj(obj)
        , r_idx(r_idx)
        , next_idx(next_idx_value)
    {
    }

    inline ~payload_deleter_t() noexcept(is_nothrow_destructible_v)
    {
      this->obj.~payload_t();
      this->r_idx = this->next_idx;
    }

    payload_t&                obj;
    std::atomic<std::size_t>& r_idx;
    std::size_t               next_idx;
  };

  std::atomic<std::size_t> m_w_idx = 0;
  mem_payload_t            m_mem[max_idx]{};
  std::atomic<std::size_t> m_r_idx = 0;
};

#include <iostream>

struct my_struct_t
{
  my_struct_t(int v)
      : a(v)
  {
    count++;
    std::cout << "c_tor_my_struct_t " << this << " " << this->a << "\n";
  }

  my_struct_t(my_struct_t const& other)
      : a(other.a)
  {
    count++;
    std::cout << "copy_ctor_my_struct_t " << this << " " << this->a << "\n";
  }

  my_struct_t(my_struct_t&& other) noexcept
      : a(other.a)
  {
    count++;
    std::cout << "move_ctor_my_struct_t " << this << " " << this->a << "\n";
  }

  my_struct_t& operator=(my_struct_t const& other) = delete;
  my_struct_t& operator=(my_struct_t&& other)      = delete;

  ~my_struct_t()
  {
    std::cout << "d_tor_my_struct_t " << this << " " << this->a << "\n";
    count--;
  }

  int        a;
  static int count;
};

int my_struct_t::count = 0;

struct a_t
{
  a_t() = default;

  a_t(a_t const&) noexcept = delete;
  a_t(a_t&&) noexcept      = default;

  a_t& operator=(a_t const&) noexcept = delete;
  a_t& operator=(a_t&&) noexcept      = delete;

  ~a_t() noexcept = default;
};

int main()
{
  {
    spsc<a_t, 1> d;

    a_t tmp;
    d.push_back(std::move(tmp));
  }

  {
    spsc<my_struct_t, 10> queue;

    constexpr bool a = noexcept(queue.push_back(std::declval<my_struct_t&&>()));
    constexpr bool b = noexcept(queue.push_back(std::declval<my_struct_t const&>()));

    std::cout << "###" << my_struct_t::count << "\n";
    std::cout << "free: " << queue.get_number_of_free_entries() << std::endl;
    std::cout << "used: " << queue.get_number_of_used_entries() << std::endl;
    for (int idx = 0; idx < 20; idx++)
    {
      std::cout << "---------\n";
      std::cout << "# " << idx << "\n";
      {
        my_struct_t tmp{ idx };
        queue.push_back(tmp);
      }
      std::cout << "free: " << queue.get_number_of_free_entries() << std::endl;
      std::cout << "used: " << queue.get_number_of_used_entries() << std::endl;
      std::cout << "---------\n";
    }
    for (int idx = 0; idx < 6; idx++)
    {
      std::cout << "---------\n";
      std::cout << "# " << idx << "\n";
      {
        auto ret = queue.pop_front();
      }
      std::cout << "free: " << queue.get_number_of_free_entries() << std::endl;
      std::cout << "used: " << queue.get_number_of_used_entries() << std::endl;
      std::cout << "---------\n";
    }
    for (int idx = 0; idx < 20; idx++)
    {
      std::cout << "---------\n";
      std::cout << "# " << idx << "\n";
      {
        my_struct_t tmp{ idx };
        queue.push_back(std::move(tmp));
      }
      std::cout << "free: " << queue.get_number_of_free_entries() << std::endl;
      std::cout << "used: " << queue.get_number_of_used_entries() << std::endl;
      std::cout << "---------\n";
    }
    for (int idx = 0; idx < 3; idx++)
    {
      std::cout << "---------\n";
      std::cout << "# " << idx << "\n";
      {
        auto ret = queue.pop_front();
      }
      std::cout << "free: " << queue.get_number_of_free_entries() << std::endl;
      std::cout << "used: " << queue.get_number_of_used_entries() << std::endl;
      std::cout << "---------\n";
    }
  }
  std::cout << "###" << my_struct_t::count << "\n";

  return 0;
}
