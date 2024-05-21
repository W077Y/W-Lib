#include <FreeRTOS.h>
#include <wlib_OS.hpp>
#include <semphr.h>
#include <stm32c0xx.h>

namespace os
{
  namespace Internal
  {
    bool is_isr() { return SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk != 0; }
    void delay(uint32_t ms) { return vTaskDelay(pdMS_TO_TICKS(ms)); }
  }    // namespace Internal

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
