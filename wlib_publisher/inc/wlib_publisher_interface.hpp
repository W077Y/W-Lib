#pragma once
#ifndef WLIB_PUBLISHER_INTERFACE_HPP
#define WLIB_PUBLISHER_INTERFACE_HPP

#include <concepts>

namespace wlib::publisher
{
  template <typename T> requires(std::is_nothrow_copy_constructible_v<T>) class Publisher_Interface
  {
  public:
    using payload_t = std::remove_cvref_t<T>;

    class Subscriber_Interface
    {
    public:
      using payload_t = Publisher_Interface::payload_t;
      virtual ~Subscriber_Interface() { m_publisher.unsubscribe(*this); }

      virtual void notify(payload_t const&) noexcept = 0;

    protected:
      Subscriber_Interface(Publisher_Interface& publisher)
          : m_publisher(publisher)
      {
        m_publisher.subscribe(*this);
      }

    private:
      friend Publisher_Interface;
      Publisher_Interface& m_publisher;
    };

  protected:
    virtual bool subscribe(Subscriber_Interface& sub)   = 0;
    virtual void unsubscribe(Subscriber_Interface& sub) = 0;
  };
}    // namespace wlib::publisher

#endif
