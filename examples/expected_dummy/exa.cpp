#include <exception>
#include <expected>
#include <iostream>
#include <new>
#include <optional>

namespace std
{
}    // namespace std

namespace t
{
  struct unexpected_t
  {
    explicit unexpected_t() = default;
  };
  inline constexpr unexpected_t unexpected{};
  
  template <typename T, typename U> class expected_t;

  template <typename T, typename U> class expected_t
  {
  public:
    using value_type                                                  = T;
    using unexpected_type                                             = U;
    template <typename Ts> static constexpr bool is_move_or_copy_able = std::is_move_constructible_v<Ts> || std::is_copy_constructible_v<Ts>;

    template <typename... Ts, typename = std::enable_if_t<std::is_constructible_v<value_type, Ts...>>>
    constexpr expected_t(Ts&&... args)
        : m_is_expected(true)
    {
      std::cout << " ex_val_const_ref_ctor_" << this;
      new (&this->m_value) value_type{ std::forward<Ts>(args)... };
    }

    template <typename... Ts, typename = std::enable_if_t<std::is_constructible_v<unexpected_type, Ts...>>>
    constexpr expected_t(unexpected_t, Ts&&... args)
        : m_is_expected(false)
    {
      std::cout << " ex_val_const_ref_ctor_" << this;
      new (&this->m_unexpected) unexpected_type{ std::forward<Ts>(args)... };
    }

    ~expected_t()
    {
      if (this->m_is_expected)
      {
        this->m_value.~value_type();
      }
      else
      {
        this->m_unexpected.~unexpected_type();
      }
      std::cout << " ex_dtor_" << this;
    }

    constexpr bool has_value() const { return this->m_is_expected; }

    constexpr value_type& value() &
    {
      std::cout << " & ";
      if (this->m_is_expected)
      {
        return this->m_value;
      }

      throw this->m_unexpected.message();
    }

    constexpr value_type const& value() const&
    {
      std::cout << " const& ";
      if (this->m_is_expected)
      {
        return this->m_value;
      }

      throw this->m_unexpected.message();
    }

    constexpr value_type&& value() &&
    {
      std::cout << " && ";
      if (this->m_is_expected)
      {
        return std::move(this->m_value);
      }

      throw this->m_unexpected.message();
    }

    constexpr value_type const&& value() const&&
    {
      std::cout << " const && ";
      if (this->m_is_expected)
      {
        return std::move(this->m_value);
      }

      throw this->m_unexpected.message();
    }

    constexpr unexpected_type const& error() const&
    {
      std::cout << " const& ";
      if (!this->m_is_expected)
      {
        return this->m_unexpected;
      }

      throw this->m_unexpected.message();
    }

    constexpr unexpected_type const&& error() const&&
    {
      std::cout << " const& ";
      if (!this->m_is_expected)
      {
        return std::move(this->m_unexpected);
      }

      throw this->m_unexpected.message();
    }

  private:
    union
    {
      value_type      m_value;
      unexpected_type m_unexpected;
    };
    bool m_is_expected;
  };

}    // namespace t

int value_to_get = 5;

class test_type
{
public:
  test_type(int& x)
      : m_val(x)
  {
    std::cout << " test_ctor_" << this;
  };

  test_type(test_type const& rhs)        = delete;
  test_type(test_type&& rhs) noexcept    = delete;
  test_type& operator=(test_type const&) = delete;
  test_type& operator=(test_type&&)      = delete;

  ~test_type() { std::cout << " test_dtor_" << this; }

  int get() const
  {
    std::cout << " get_" << this << " ";
    return this->m_val;
  }

private:
  int& m_val;
};

class error_t
{
public:
  error_t(char const* msg)
      : m_msg(msg)
  {
    std::cout << " error_ctor_" << this;
  }

  error_t(error_t const&)            = delete;
  error_t(error_t&&)                 = delete;
  error_t& operator=(error_t const&) = delete;
  error_t& operator=(error_t&&)      = delete;

  ~error_t() { std::cout << " error_dtor_" << this; }

  char const* message() const { return this->m_msg; }

private:
  char const* m_msg;
};

t::expected_t<test_type, error_t> fnc(int const& i)
{
  if (i < 0)
    return { t::unexpected, "error a" };
  if (i < 2)
  {
    return { value_to_get };
  }
  return { t::unexpected, "error b" };
}

std::optional<test_type> fnc2(int const& i)
{
  if (i < 0)
    return std::nullopt;
  if (i < 5)
    return { value_to_get };
  return std::nullopt;
}

int main()
{
  std::cout << "copy const:" << std::is_copy_constructible_v<test_type> << std::endl;
  std::cout << "move const:" << std::is_move_constructible_v<test_type> << std::endl;

  {
    auto&& aaa = fnc2(0).value();
    aaa.get();
  }
  std::cout << std::endl;

  using T = t::expected_t<test_type, error_t>;
  for (int idx = -5; idx < 10; idx++)
  {
    std::cout << idx << ": ";
    {
      auto tmp = fnc(idx);
      if (tmp.has_value())
      {
        std::cout << " has_value " << tmp.value().get() << std::endl;
      }
      else
      {
        std::cout << " has_error " << tmp.error().message() << std::endl;
      }
    }
    std::cout << std::endl;
  }
  std::cout << "########" << std::endl;
  for (int idx = 0; idx < 10; idx++)
  {
    std::cout << idx << ": ";
    {
      try
      {
        std::cout << " : ";
        {
          auto&& tmp = fnc(idx).value();
          std::cout << " has_value ";
          std::cout << tmp.get();
        }
        std::cout << " : " << std::endl;
      }
      catch (char const* msg)
      {
        std::cout << " throw " << msg << std::endl;
      }
      catch (...)
      {
        std::cout << " throw " << std::endl;
      }
    }
    std::cout << std::endl;
  }
}
