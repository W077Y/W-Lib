#pragma once
#ifndef WLIB_CALLBACK_HPP
#define WLIB_CALLBACK_HPP

namespace wlib
{
  template <typename> class Callback;
  template <typename R, typename... Args> class Callback<R(Args...)>
  {
  public:
    using signature_t = R(Args...);
    virtual ~Callback() = default;

    virtual R operator()(Args...) = 0;
  };
}    // namespace wlib

namespace wlib
{
  template <typename> class Function_Callback;
  template <typename R, typename... Args> class Function_Callback<R(Args...)> final: public Callback<R(Args...)>
  {
    using fnc_t = R (&)(Args...);

  public:
    constexpr Function_Callback(R (&fnc)(Args...))
        : m_fnc{ fnc }
    {
    }

    R operator()(Args... args) override { return this->m_fnc(args...); }

  private:
    fnc_t m_fnc;
  };
}    // namespace wlib

namespace wlib
{
  template <typename, typename> class Memberfunction_Callback;
  template <typename T, typename R, typename... Args> class Memberfunction_Callback<T, R(Args...)> final: public Callback<R(Args...)>
  {
  public:
    constexpr Memberfunction_Callback(T& obj, R (T::*mem_fuc)(Args...))
        : m_obj(obj)
        , m_mem_fuc(mem_fuc)
    {
    }

    R operator()(Args... args) override { return (this->m_obj.*m_mem_fuc)(args...); }

  private:
    T& m_obj;
    R  (T::*m_mem_fuc)(Args...);
  };
}    // namespace wlib

#endif
