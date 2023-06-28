#pragma once
#ifndef WLIB_FUNCTION_CALLBACK_HPP
#define WLIB_FUNCTION_CALLBACK_HPP

#include <inc/Callback_Interface_utility.hpp>

namespace WLib
{
  template <typename> class Function_Callback;
  template <typename R, typename... Args> class Function_Callback<R(Args...)> final: public Callback_Interface<R(Args...)>
  {
    using fnc_t = R (&)(Args...);

  public:
    constexpr Function_Callback(R (&fnc)(Args...))
        : m_fnc{ fnc }
    {
    }

    R operator()(Args... args) override { return this->m_fnc(args ...); }

  private:
    fnc_t m_fnc;
  };
}    // namespace WLib
#endif
