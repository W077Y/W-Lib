#pragma once
#ifndef WLIB_OS_HPP
#define WLIB_OS_HPP

#include <FreeRTOS.h>
#include <chrono>
#include <cstddef>
#include <limits>
#include <semphr.h>
#include <task.h>
#include <wlib_callback.hpp>

namespace os
{
  class Task
  {
    static void  task_handle(void* obj);
    static void* create_task(char const* name, uint32_t const& min_stack_size_bytes, Task& task);

  public:
    template <typename... Args> class handle_t final: public wlib::Callback<void()>
    {
      void                (&m_fnc)(Args...);
      std::tuple<Args...> m_param;

    public:
      handle_t(void (&fnc)(Args...), std::tuple<Args...> args)
          : m_fnc(fnc)
          , m_param(args)
      {
      }

      void operator()() override { return std::apply(this->m_fnc, this->m_param); }
    };

    template <typename... Args> Task(char const* name, uint32_t const& min_stack_size_bytes, void (&handle)(Args...), std::tuple<Args...> args)
    {
      static_assert(sizeof(handle_t<Args...>) < sizeof(m_obj_mem));

      if (new (&this->m_obj_mem[0]) handle_t<Args...>(handle, args) == nullptr)
      {
        // TODO: error
      }
      this->handle = create_task(name, min_stack_size_bytes, *this);
    }

    Task(char const* name, uint32_t const& min_stack_size_bytes, void (&handle)())
    {
      if (new (&this->m_obj_mem[0]) wlib::Function_Callback<void()>(handle) == nullptr)
      {
        // TODO: error
      }
      this->handle = create_task(name, min_stack_size_bytes, *this);
    }

  private:
    void*    handle = nullptr;
    uint32_t m_obj_mem[10]{};
  };

  namespace internal
  {
    bool is_isr() noexcept;
    void delay_until(std::chrono::steady_clock::time_point const& time_point) noexcept;
  }    // namespace internal

  namespace this_thread
  {
    void yield() noexcept;

    template <class Rep, class Period> void sleep_for(const std::chrono::duration<Rep, Period>& sleep_duration)
    {
      return internal::delay_until(std::chrono::steady_clock::now() + sleep_duration);
    }
    template <class Clock, class Duration> void sleep_until(const std::chrono::time_point<Clock, Duration>& sleep_time)
    {
      return internal::delay_until(std::chrono::steady_clock::now() + (sleep_time - Clock::now()));
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
      if (internal::is_isr())
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
    void* m_handle{};
  };

  using binary_semaphore = counting_semaphore<1>;
}    // namespace os

#endif
