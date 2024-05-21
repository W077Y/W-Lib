#pragma once
#ifndef WLIB_PIN_ABSTRACTION_HPP
#define WLIB_PIN_ABSTRACTION_HPP

namespace wlib::abstraction
{
  class Digital_Input_Interface
  {
  public:
    virtual ~Digital_Input_Interface() = default;

    virtual bool get() = 0;
  };

  class Digital_Output_Interface: public Digital_Input_Interface
  {
  public:
    virtual ~Digital_Output_Interface() = default;

    virtual void set_high() = 0;
    virtual void set_low()  = 0;
    virtual void toggle()   = 0;

    virtual void set(bool val)
    {
      if (val)
        return this->set_high();
      return this->set_low();
    }
  };
}    // namespace wlib::abstraction

#endif
