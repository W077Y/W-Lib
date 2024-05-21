#pragma once
#ifndef WLIB_OS_HPP
#define WLIB_OS_HPP

#include <atomic>
#include <chrono>
#include <cstddef>
#include <limits>

namespace os
{
  namespace this_thread
  {
    void yield() noexcept;

    template <typename Clock, typename Duration> inline void sleep_until(std::chrono::time_point<Clock, Duration> const& deadline)
    {
      static_assert(std::chrono::is_clock_v<Clock>);

      while (Clock::now() < deadline)
        yield();
    }

    template <typename Rep, typename Period> inline void sleep_for(std::chrono::duration<Rep, Period> const& duration)
    {
      sleep_until(std::chrono::steady_clock::now() + duration);
    }
  }    // namespace this_thread
}    // namespace os

namespace os
{
  template <uint16_t LeastMaxValue = std::numeric_limits<uint16_t>::max()> class counting_semaphore
  {
  public:
    counting_semaphore(uint16_t init_value)
        : m_val(init_value)
    {
    }

    ~counting_semaphore() {}

    void release()
    {
      while (true)
      {
        uint32_t       val      = this->m_val;
        uint32_t const next_val = val + 1;
        if (next_val > LeastMaxValue)
        {
          while (true)
          {
          }
          //__asm("bkpt 255");
          //__asm("bx lr");
          // throw "";
        }

        if (this->m_val.compare_exchange_weak(val, next_val))
          return;
      }
    }

    void acquire()
    {
      while (true)
      {
        uint32_t val = this->m_val;
        if (val == 0)
          continue;

        uint32_t const next = val - 1;
        if (this->m_val.compare_exchange_weak(val, next))
          return;
      }
    }

    bool try_acquire()
    {
      while (true)
      {
        uint32_t val = this->m_val;
        if (val == 0)
          return false;

        uint32_t const next_val = val - 1;
        if (this->m_val.compare_exchange_weak(val, next_val))
          return true;
      }
    }

    template <class Rep, class Period> bool try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time);

    template <class Clock, class Duration> bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& abs_time);

  private:
    std::atomic<uint32_t> m_val;
  };

  using binary_semaphore = counting_semaphore<1>;
}    // namespace os

#endif
