#include <WLib_LockFree_Puplisher.hpp>
#include <WLib_SPSC_Subscriber.hpp>
#include <WLib_Utility.hpp>
#include <iostream>
#include <semaphore>
#include <thread>

namespace rtos
{
  namespace this_thread
  {
    using std::this_thread::yield;
  }
}    // namespace rtos

//

struct MyStruct
{
};

class subscriber: public WLib::Publisher_Interface<int>::Subscriber_Interface
{
public:
  subscriber(WLib::Publisher_Interface<int>& pub, int idx)
      : WLib::Publisher_Interface<int>::Subscriber_Interface(pub)
      , m_idx(idx)
  {
  }

private:
  int  m_idx;
  void notify(payload_t const& val) noexcept override { std::cout << this->m_idx << " | " << val << "\n"; }
};

class value_pub: public WLib::LockFree_Publisher_Base<int, 5>
{
public:
  value_pub()
  {
    this->m_worker = std::thread([&]() { this->task(); });
  }

  ~value_pub()
  {
    this->m_run = false;
    this->m_worker.join();
  }

private:
  std::thread       m_worker;
  std::atomic<bool> m_run = true;

  void task()
  {
    int value = 0;
    while (this->m_run)
    {
      this->notify_all_subscribers(value++);
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
};

int main()
{
  value_pub pub;

  {
    WLib::SPSC_Subscriber<int, 8> list_sub{ pub };
    subscriber                      sub1{ pub, 1 };
    {
      subscriber sub2{ pub, 2 };
      subscriber sub3{ pub, 3 };

      std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));

    for (auto o_val = list_sub.try_get_value(); o_val.has_value(); o_val = list_sub.try_get_value())
    {
      std::cout << "l"
                << " | " << o_val.value() << "\n";
    }
  }

  std::cout << "This is a demo example!" << std::endl;
  return 0;
}
