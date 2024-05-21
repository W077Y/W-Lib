#include <wlib_LED_abstraction.hpp>

namespace wlib::abstraction
{
  void TrafficLight_PIN::set_state(State const& new_state)
  {
    this->m_pin_r.set_low();
    this->m_pin_y.set_low();
    this->m_pin_g.set_low();

    switch (new_state)
    {
    case State::Run:
      this->m_pin_g.set_high();
      break;

    case State::Warning:
      this->m_pin_y.set_high();
      break;

    case State::Error:
      this->m_pin_r.set_high();
      break;

    default:
      break;
    }
  }
}    // namespace wlib::abstraction
