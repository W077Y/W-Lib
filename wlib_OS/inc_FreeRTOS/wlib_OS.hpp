#pragma once
#ifndef WLIB_OS_HPP
#define WLIB_OS_HPP

#include <FreeRTOS.h>
#include <chrono>
#include <cstddef>
#include <limits>
#include <semphr.h>
#include <task.h>

namespace os
{
  namespace Internal
  {
    bool is_isr() noexcept;
    void delay(uint32_t ms);
  }

  namespace this_thread
  {
    void                                    yield() noexcept;
    template <class Rep, class Period> void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration)
    {
      Internal::delay(static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(sleep_duration).count()));
    }
  }    // namespace this_thread

  template <uint16_t LeastMaxValue = std::numeric_limits<uint16_t>::max()> class counting_semaphore
  {
  public:
    counting_semaphore(uint16_t init_value)
        : m_handle(xSemaphoreCreateCounting(LeastMaxValue, init_value))
    {
    }

    ~counting_semaphore() { vSemaphoreDelete(this->m_handle); }

    void release()
    {
      if (Internal::is_isr())
      {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(this->m_handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
      }
      else
      {
        xSemaphoreGive(this->m_handle);
      }
    }

    void acquire() { xSemaphoreTake(this->m_handle, portMAX_DELAY); }

    bool try_acquire() { return xSemaphoreTake(this->m_handle, pdMS_TO_TICKS(0)) == pdTRUE; }

    template <class Rep, class Period> bool try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time);

    template <class Clock, class Duration> bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& abs_time);

  private:
    SemaphoreHandle_t m_handle;
  };

  using binary_semaphore = counting_semaphore<1>;
}    // namespace os

#endif
