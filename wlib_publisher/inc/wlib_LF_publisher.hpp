#pragma once
#ifndef WLIB_LOCKFREE_PUBLISHER_HPP
#define WLIB_LOCKFREE_PUBLISHER_HPP

#include <atomic>
#include <concepts>
#include <cstddef>
#include <wlib_OS.hpp>
#include <wlib_publisher_interface.hpp>

namespace wlib::publisher
{
  template <typename T, std::size_t N> requires(N > 0) class LF_Publisher: public Publisher_Interface<T>
  {
  public:
    using payload_t            = typename Publisher_Interface<T>::payload_t;
    using Subscriber_Interface = typename Publisher_Interface<T>::Subscriber_Interface;

    static constexpr std::size_t max_number_of_subscribers = N;

  public:
    void notify_all_subscribers(payload_t const& value) noexcept
    {
      this->m_notify_ongoing = true;
      for (auto& cur_entry : m_subscriber_list)
      {
        Subscriber_Interface* subscriber = cur_entry.load(std::memory_order_acquire);
        if (subscriber != nullptr)
        {
          subscriber->notify(value);
        }
      }
      this->m_notify_ongoing = false;
    }

    bool subscribe(Subscriber_Interface& sub) override
    {
      for (auto& cur_entry : m_subscriber_list)
      {
        Subscriber_Interface* expected = nullptr;
        if (cur_entry.compare_exchange_strong(expected, &sub))
        {
          return true;
        }
      }
      return false;
    }

    void unsubscribe(Subscriber_Interface& sub) override
    {
      for (auto& cur_entry : m_subscriber_list)
      {
        Subscriber_Interface* expected = &sub;
        cur_entry.compare_exchange_strong(expected, nullptr);
      }
      while (this->m_notify_ongoing)
      {
        os::this_thread::yield();
      }
    }

  private:
    std::atomic<Subscriber_Interface*> m_subscriber_list[max_number_of_subscribers] = {};
    std::atomic<bool>                  m_notify_ongoing                             = false;
  };
}    // namespace wlib::publisher

#endif
