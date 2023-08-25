#include <exception>
#include <iostream>
#include <new>
#include <optional>

class error_t final
{
public:
  static constexpr error_t get_no_error() { return error_t{}; }

  explicit constexpr error_t(char const* msg)
      : m_msg(msg)
  {
  }

  bool                  is_no_error() const { return this->m_msg == nullptr; }
  constexpr char const* get_msg() const { return this->m_msg; }

private:
  explicit constexpr error_t() = default;

  char const* m_msg = nullptr;
};

class error_exception final: public std::exception
{
public:
  error_exception(error_t const& err)
      : m_msg(err.get_msg())
  {
  }

  virtual char const* what() const override { return this->m_msg; }

private:
  char const* m_msg = nullptr;
};

error_t const no_err = error_t::get_no_error();

template <typename T> class ret_t
{
public:
  constexpr ret_t(error_t const& err)
      : m_err(err)
      , m_val{}
  {
  }

  constexpr ret_t(T const& val) { new (this->m_val) T(val); }
  constexpr ret_t(T&& val) { new (this->m_val) T(val); }

  template <typename... Ts> constexpr ret_t(std::in_place_t, Ts&&... args) { new (this->m_val) T(args...); }

  ~ret_t()
  {
    if (this->has_value())
    {
      reinterpret_cast<T*>(this->m_val)->~T();
    }
  }

  bool    has_value() const { return this->m_err.is_no_error(); }
  error_t get_err() const { return this->m_err; }

  T const& value() const&
  {
    if (this->m_err.is_no_error())
      return *reinterpret_cast<T const*>(this->m_val);
    throw error_exception(this->m_err);
  }

  T get_value() const&&
  {
    if (this->m_err.is_no_error())
      return *reinterpret_cast<T const*>(this->m_val);
    throw error_exception(this->m_err);
  }

private:
  error_t m_err = no_err;
  alignas(T) char m_val[sizeof(T)];
};

error_t err_a{ "err a" };

struct MyStruct
{
  MyStruct(char const* val)
      : val(val)
  {
    std::cout << "ctor ";
  }

  MyStruct(MyStruct const& val) = delete;
  MyStruct(MyStruct&& val)      = delete;

  ~MyStruct() { std::cout << "dtor "; }

  char const* val;
};

ret_t<MyStruct> fnc(int i)
{
  if (i > 0)
    return { std::in_place, "gut" };
  return err_a;
}

// std::optional<MyStruct> fnc(int i)
//{
//   if (i > 0)
//     return { "gut" };
//   return std::nullopt;
// }

int main()
{
  try
  {
    std::cout << fnc(2).value().val << std::endl;

    if (auto val = fnc(2); val.has_value())
    {
      char const* txt = val.value().val;
      std::cout << txt << std::endl;
    }
    else
    {
      std::cout << "ret_err " << val.get_err().get_msg() << std::endl;
    }

    if (auto val = fnc(-3); val.has_value())
      std::cout << val.value().val << std::endl;
    else
    {
      std::cout << "ret_err " << val.get_err().get_msg() << std::endl;
    }

    std::cout << fnc(-2).value().val << std::endl;
  }
  catch (std::exception& ex)
  {
    std::cout << "catch_err " << ex.what() << std::endl;
  }
}
