#include <atomic>
#include <cstdint>
#include <optional>
#include <type_traits>

template <typename T, std::size_t N> class spsc
{
public:
  using payload_t = std::remove_cv_t<T>;

  spsc() = default;

  ~spsc()
  {
    std::size_t const w = this->m_w_idx;
    std::size_t       r = this->m_r_idx;
    while (r != w)
    {
      payload_t& tmp = *std::launder(reinterpret_cast<payload_t*>(&this->m_mem[r]));
      tmp.~payload_t();
      r = advance(w, 1);
    }
    this->m_r_idx = r;
  }

  bool push_back(payload_t const& v) noexcept { return this->p_push_back<payload_t const&, false>(v); }
  bool push_back(payload_t&& v) noexcept { return this->p_push_back<payload_t&&, true>(std::move(v)); }

  bool pop_front(payload_t& v) noexcept
  {
    std::size_t const w = this->m_w_idx;
    std::size_t const r = this->m_r_idx;

    if (r == w)
      return false;

    payload_deleter_t tmp{ *std::launder(reinterpret_cast<payload_t*>(&this->m_mem[r])), this->m_r_idx, advance(r, 1) };
    if constexpr (std::is_move_assignable_v<payload_t>)
    {
      v = std::move(tmp.obj);
    }
    else if constexpr (std::is_copy_assignable_v<payload_t>)
    {
      v = tmp.obj;
    }
    else if constexpr (std::is_move_constructible_v<payload_t>)
    {
      v.~payload_t();
      ::new (&v) payload_t(std::move(tmp.obj));
    }
    else
    {
      v.~payload_t();
      ::new (&v) payload_t(tmp.obj);
    }
    return true;
  }

  std::optional<payload_t> pop_front() noexcept
  {
    std::size_t const w = this->m_w_idx;
    std::size_t const r = this->m_r_idx;

    if (r == w)
      return std::nullopt;

    payload_deleter_t tmp{ *std::launder(reinterpret_cast<payload_t*>(&this->m_mem[r])), this->m_r_idx, advance(r, 1) };
    if constexpr (std::is_move_constructible_v<payload_t>)
    {
      return { std::move(tmp.obj) };
    }
    else
    {
      return { tmp.obj };
    }
  }

private:
  static_assert(std::is_copy_constructible_v<payload_t> || std::is_move_constructible_v<payload_t>);

  using mem_payload_t = std::aligned_storage_t<sizeof(payload_t), alignof(payload_t)>;

  static constexpr std::size_t max_idx = N + 1;

  template <typename TT, bool move_obj> inline bool p_push_back(TT v)
  {
    std::size_t const r      = this->m_r_idx;
    std::size_t const w      = this->m_w_idx;
    std::size_t const w_next = advance(w, 1);

    if (w_next == r)
      return false;

    if constexpr (move_obj)
    {
      ::new (&this->m_mem[w]) payload_t(std::move(v));
    }
    else
    {
      ::new (&this->m_mem[w]) payload_t(v);
    }

    this->m_w_idx = w_next;
    return true;
  }

  static inline constexpr std::size_t advance(std::size_t idx, std::size_t val)
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
    inline payload_deleter_t(payload_t& obj, std::atomic<std::size_t>& r_idx, std::size_t next_idx_value)
        : obj(obj)
        , r_idx(r_idx)
        , next_idx(next_idx_value)
    {
    }

    inline ~payload_deleter_t()
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

int main()
{
  spsc<my_struct_t, 10> queue;

  std::cout << "###" << my_struct_t::count << "\n";
  for (int idx = 0; idx < 3; idx++)
  {
    std::cout << "# " << idx << "\n";
    my_struct_t tmp{ idx };
    queue.push_back(tmp);
  }
  for (int idx = 3; idx < 6; idx++)
  {
    std::cout << "# " << idx << "\n";
    my_struct_t tmp{ idx };
    queue.push_back(std::move(tmp));
  }
  std::cout << "###" << my_struct_t::count << "\n";
  for (int idx = 0; idx < 3; idx++)
  {
    std::cout << "# " << idx << "\n";
    my_struct_t tmp{ -1 };
    queue.pop_front(tmp);
  }
  for (int idx = 3; idx < 6; idx++)
  {
    std::cout << "# " << idx << "\n";
    queue.pop_front();
  }

  std::cout << "###" << my_struct_t::count << "\n";

  return 0;
}
