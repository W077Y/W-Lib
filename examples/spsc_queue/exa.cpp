#include <WLib_SPSC.hpp>
#include <atomic>
#include <cstdint>
#include <iostream>
#include <optional>
#include <type_traits>

struct my_struct_t
{
  my_struct_t(int v)
      : a(v)
  {
    count++;
    std::cout << "c_tor_my_struct_t " << this << " " << this->a << "\n";
  }

  my_struct_t(my_struct_t const& other)
      : a(other.a)
  {
    count++;
    std::cout << "copy_ctor_my_struct_t " << this << " " << this->a << "\n";
  }

  my_struct_t(my_struct_t&& other) noexcept
      : a(other.a)
  {
    count++;
    std::cout << "move_ctor_my_struct_t " << this << " " << this->a <<"\n";
  }

  my_struct_t& operator=(my_struct_t const& other) = delete;
  my_struct_t& operator=(my_struct_t&& other)      = delete;

  ~my_struct_t()
  {
    std::cout << "d_tor_my_struct_t " << this << " " << this->a << "\n";
    count--;
  }

  int        a;
  static int count;
};

int my_struct_t::count = 0;

static_assert(std::is_destructible_v<my_struct_t>);
static_assert(std::is_copy_constructible_v<my_struct_t>);
static_assert(std::is_move_constructible_v<my_struct_t>);
static_assert(!std::is_nothrow_copy_constructible_v<my_struct_t>);
static_assert(std::is_nothrow_move_constructible_v<my_struct_t>);

int main()
{
  {
    WLib::Container::spsc<my_struct_t, 10> queue;

    std::cout << "###" << my_struct_t::count << "\n";
    std::cout << "free: " << queue.get_number_of_free_entries() << std::endl;
    std::cout << "used: " << queue.get_number_of_used_entries() << std::endl;
    for (int idx = 0; idx < 20; idx++)
    {
      std::cout << "---------\n";
      std::cout << "# " << idx << "\n";
      {
        my_struct_t tmp{ idx };
        queue.push_back(tmp);
      }
      std::cout << "free: " << queue.get_number_of_free_entries() << std::endl;
      std::cout << "used: " << queue.get_number_of_used_entries() << std::endl;
      std::cout << "---------\n";
    }
    for (int idx = 0; idx < 6; idx++)
    {
      std::cout << "---------\n";
      std::cout << "# " << idx << "\n";
      {
        auto ret = queue.pop_front();
      }
      std::cout << "free: " << queue.get_number_of_free_entries() << std::endl;
      std::cout << "used: " << queue.get_number_of_used_entries() << std::endl;
      std::cout << "---------\n";
    }
    for (int idx = 0; idx < 20; idx++)
    {
      std::cout << "---------\n";
      std::cout << "# " << idx << "\n";
      {
        my_struct_t tmp{ idx };
        queue.push_back(std::move(tmp));
      }
      std::cout << "free: " << queue.get_number_of_free_entries() << std::endl;
      std::cout << "used: " << queue.get_number_of_used_entries() << std::endl;
      std::cout << "---------\n";
    }
    for (int idx = 0; idx < 3; idx++)
    {
      std::cout << "---------\n";
      std::cout << "# " << idx << "\n";
      {
        auto ret = queue.pop_front();
      }
      std::cout << "free: " << queue.get_number_of_free_entries() << std::endl;
      std::cout << "used: " << queue.get_number_of_used_entries() << std::endl;
      std::cout << "---------\n";
    }
  }
  std::cout << "###" << my_struct_t::count << "\n";

  return 0;
}
