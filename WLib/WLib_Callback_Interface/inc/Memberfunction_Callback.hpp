#pragma once
#ifndef WLIB_MEMBERFUNCTION_CALLBACK_HPP
#define WLIB_MEMBERFUNCTION_CALLBACK_HPP

#include <inc/Callback_Interface_utility.hpp>

namespace WLib
{
  template <typename, typename> class Memberfunction_Callback;
  template <typename T, typename R, typename... Args> class Memberfunction_Callback<T, R(Args...)> final : public Callback_Interface<R(Args...)>
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
    R (T::*m_mem_fuc)(Args...);
  };
} // namespace WLib
#endif
