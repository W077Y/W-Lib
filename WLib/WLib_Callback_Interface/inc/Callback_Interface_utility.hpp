#pragma once
#ifndef WLIB_CALLBACK_INTERFACE_UTILITY_HPP
#define WLIB_CALLBACK_INTERFACE_UTILITY_HPP

namespace WLib
{
  template <typename> class Callback_Interface;
  template <typename R, typename... Args> class Callback_Interface<R(Args...)>
  {
  public:
    virtual ~Callback_Interface() = default;

    virtual R operator()(Args...) = 0;
  };
} // namespace WLib
#endif
