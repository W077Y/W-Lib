#pragma once
#ifndef WLIB_PWM_ABSTRACTION_HPP
#define WLIB_PWM_ABSTRACTION_HPP

namespace wlib::abstraction
{
  class PWM_Interface
  {
  public:
    virtual ~PWM_Interface() = default;

    virtual float operator()(float const& ratio) = 0;
  };
}    // namespace wlib::abstraction

#endif
