#include <WLib_Puplisher_Inteface.hpp>

#include <iostream>
#include <mutex>
#include <type_traits>

////////////////////////

template <typename T> class puplisher_t: public WLib::Puplisher_Base<T>
{
public:
  using payload_t = typename WLib::Puplisher_Base<T>::payload_t;

  virtual void puplish(payload_t const& value) override
  {
    std::lock_guard<std::mutex> lock{ this->m_tex };
    return this->p_puplish(value);
  }

  using WLib::Puplisher_Base<T>::subscribe;

private:
  virtual void add(WLib::Subscriber_Interface<T>& node) override
  {
    std::lock_guard<std::mutex> lock{ this->m_tex };
    return this->p_add(node);
  }

  virtual void remove(WLib::Subscriber_Interface<T>& node) override
  {
    std::lock_guard<std::mutex> lock{ this->m_tex };
    return this->p_remove(node);
  }
  std::mutex m_tex;
};

template <typename T> class sub_handle_t: public WLib::Subscriber_Interface<T>
{
public:
  sub_handle_t(WLib::Subscription_Interface<T>& parent)
      : WLib::Subscriber_Interface<T>(parent)
  {
  }

  T get_value()
  {
    std::lock_guard<std::mutex> lock{ this->m_tex };
    return this->m_value;
  }

private:
  virtual void receive_data(T const& value) override
  {
    std::lock_guard<std::mutex> lock{ this->m_tex };
    this->m_value = value;
  }
  std::mutex m_tex;
  T          m_value = 0;
};

#include <chrono>
#include <thread>
#include <atomic>

puplisher_t<int>& func()
{
  static puplisher_t<int> obj;
  return obj;
}

int main()
{
  std::atomic<bool> run_prod = true;
  auto              prod     = [&]()
  {
    puplisher_t<int>& pup = func();
    int               i   = 0;
    while (run_prod)
    {
      pup.puplish(i++);
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  };

  std::atomic<bool> run  = true;
  auto              cons = [&](const char* name)
  {
    auto handle = func().subscribe<sub_handle_t<int>>();
    while (run)
    {
      std::cout << name << " " << handle.get_value() << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(75));
    }
  };
  auto cons2 = [&](const char* name)
  {
    auto handle = func().subscribe<sub_handle_t<int>>();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << name << " " << handle.get_value() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(75));
  };

  std::thread th_prod = std::thread(prod);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::thread th_cons_1 = std::thread([&]() { cons("cons A"); });
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  std::thread th_cons_2 = std::thread([&]() { cons("cons B"); });
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));
  std::thread th_cons_3 = std::thread([&]() { cons2("cons C"); });
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  run = false;
  th_cons_1.join();
  th_cons_2.join();
  th_cons_3.join();

  run_prod = false;
  th_prod.join();
  std::cout << "Ende";
  return 0;
}
