#include <FreeRTOS.h>
#include <atomic>
#include <semphr.h>
#include <stm32h753xx.h>
#include <wlib_OS.hpp>

namespace
{
  std::atomic<uint32_t> chrono_val_low  = 0;
  std::atomic<uint32_t> chrono_val_high = 0;
}    // namespace

std::chrono::steady_clock::time_point std::chrono::steady_clock::now() noexcept
{
  using T = std::chrono::steady_clock;
  while (true)
  {
    uint32_t const h   = chrono_val_high;
    uint32_t const l   = chrono_val_low;
    uint32_t const sub = ((480'000 - SysTick->VAL) * 1'000) / 480;

    if (l == chrono_val_low.load() && h == chrono_val_high.load())
    {
      uint64_t const val = (((static_cast<uint64_t>(h) << 32) + static_cast<uint64_t>(l)) * 1'000'000) + sub;
      return T::time_point(T::duration(std::chrono::nanoseconds(val)));
    }
  }
}

std::chrono::system_clock::time_point std::chrono::system_clock::now() noexcept
{
  using T = std::chrono::system_clock;
  return T::time_point(T::duration(std::chrono::nanoseconds(std::chrono::steady_clock::now().time_since_epoch().count())));
}

extern "C" void vAssertCalled([[maybe_unused]] uint32_t ulLine, [[maybe_unused]] const char* pcFile)
{
  __asm("bkpt 255");
  __asm("bx lr");
}

extern "C" void vApplicationStackOverflowHook([[maybe_unused]] TaskHandle_t xTask, [[maybe_unused]] char* pcTaskName)
{
  __asm("bkpt 255");
  __asm("bx lr");
}

extern "C" void vApplicationMallocFailedHook()
{
  __asm("bkpt 255");
  __asm("bx lr");
}

extern "C" void vApplicationTickHook()
{
  uint32_t const l_new = chrono_val_low + 1;
  chrono_val_low       = l_new;
  if (l_new == 0)
    chrono_val_high++;
}

extern "C" void start_os(void (*handle)(void*))
{
  xTaskCreate(handle, "main", 1024, nullptr, tskIDLE_PRIORITY, nullptr);
  vTaskStartScheduler();
}

namespace os
{
  void Task::task_handle(void* obj)
  {
    Task& self = *reinterpret_cast<Task*>(obj);
    reinterpret_cast<wlib::Callback<void()>*>(&self.m_obj_mem[0])->operator()();
    vTaskDelete(nullptr);
  }

  void* Task::create_task(char const* name, uint32_t const& min_stack_size_bytes, Task& task)
  {
    uint16_t stack_size = (min_stack_size_bytes + sizeof(StackType_t) - 1) / sizeof(StackType_t);
    if (stack_size < configMINIMAL_STACK_SIZE)
      stack_size = configMINIMAL_STACK_SIZE;

    TaskHandle_t ret = nullptr;
    if (xTaskCreate(task_handle, name, stack_size, reinterpret_cast<void*>(&task), tskIDLE_PRIORITY, &ret) != pdPASS)
    {
      // TODO: error
    }
    return ret;
  }

  namespace internal
  {
    bool is_isr() noexcept { return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0; }
    void delay_until(std::chrono::steady_clock::time_point const& time_point) noexcept
    {
      constexpr int64_t max_time = portMAX_DELAY - 1;
      int64_t           wait_time =
          std::chrono::duration_cast<std::chrono::milliseconds>((time_point - std::chrono::steady_clock::now()) + std::chrono::milliseconds(1)).count();
      while (wait_time >= max_time)
      {
        vTaskDelay(max_time);
        wait_time =
            std::chrono::duration_cast<std::chrono::milliseconds>((time_point - std::chrono::steady_clock::now()) + std::chrono::milliseconds(1)).count();
      }
      vTaskDelay(wait_time);
    }    // namespace internal
  }      // namespace internal
  namespace this_thread
  {
    void yield() noexcept { taskYIELD(); }
  }    // namespace this_thread

  // template <uint16_t LeastMaxValue = std::numeric_limits<uint16_t>::max()> class counting_semaphore
  //{
  // public:
  //   struct native_handle;

  //  counting_semaphore(uint16_t init_value);
  //
  //  ~counting_semaphore();

  //  void release();

  //  void acquire();

  //  bool try_acquire();

  //  template <class Rep, class Period> bool try_acquire_for(const std::chrono::duration<Rep, Period>& rel_time);

  //  template <class Clock, class Duration> bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& abs_time);

  // private:
  //   native_handle* m_handle;
  // };

  // using binary_semaphore = counting_semaphore<1>;
}    // namespace os
