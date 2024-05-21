#pragma once
#ifndef WLIB_LED_ABSTRACTION_HPP
#define WLIB_LED_ABSTRACTION_HPP

#include <wlib_PIN_abstraction.hpp>

namespace wlib::abstraction
{
  class LED_Interface
  {
  public:
    virtual ~LED_Interface() = default;
    virtual void on()        = 0;
    virtual void off()       = 0;
    virtual void toggle()    = 0;
  };

  class LED_PIN final: public LED_Interface
  {
  public:
    LED_PIN(Digital_Output_Interface& pin)
        : m_pin(pin)
    {
    }

    void on() override { this->m_pin.set_high(); }
    void off() override { this->m_pin.set_low(); }
    void toggle() override { this->m_pin.toggle(); }

  private:
    Digital_Output_Interface& m_pin;
  };

  class TrafficLight_Interface
  {
  public:
    enum class State
    {
      Off,
      Error,
      Warning,
      Run,
    };
    virtual void set_state(State const&) = 0;
  };

  class TrafficLight_PIN final: public TrafficLight_Interface
  {
  public:
    TrafficLight_PIN(Digital_Output_Interface& pin_red, Digital_Output_Interface& pin_yellow, Digital_Output_Interface& pin_green)
        : m_pin_r(pin_red)
        , m_pin_y(pin_yellow)
        , m_pin_g(pin_green)
    {
    }

    virtual void set_state(State const&) override;

  private:
    Digital_Output_Interface& m_pin_r;
    Digital_Output_Interface& m_pin_y;
    Digital_Output_Interface& m_pin_g;
  };
}    // namespace wlib::abstraction

#endif
