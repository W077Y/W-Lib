#include <WLib_Callback_Interface.hpp>
#include <iostream>

int fnc_a(int lhs, int rhs) { return 2 * lhs + 3 * rhs; }
int fnc_b(int lhs, int rhs) { return 5 * lhs + 7 * rhs; }

class call_obj final: public WLib::Callback_Interface<int(int, int)>
{
public:
  call_obj(int val)
      : m_val(val)
  {
  }

  int operator()(int lhs, int rhs) override { return lhs + rhs + this->m_val; }

private:
  int m_val = 0;
};

void function(WLib::Callback_Interface<int(int, int)>& cb, const char* name)
{
  int const a = 3;
  int const b = 4;
  int const c = cb(a, b);

  std::cout << name << ": f(3,4) = " << c << std::endl;
}

class obj
{
public:
  obj(int val)
      : m_val(val)
  {
  }

  WLib::Memberfunction_Callback<obj, int(int, int)> get_cb_1() { return { *this, &obj::fnc_1 }; }
  WLib::Memberfunction_Callback<obj, int(int, int)> get_cb_2() { return { *this, &obj::fnc_2 }; }

private:
  int fnc_1(int a, int b) { return this->m_val + a - b; }
  int fnc_2(int a, int b) { return this->m_val - a - b; }

  int m_val = 0;
};

int main()
{

  WLib::Function_Callback<int(int, int)> a{ fnc_a };
  WLib::Function_Callback<int(int, int)> b{ fnc_b };
  
  call_obj                               c{ 3 };
  call_obj                               d{ 4 };

  obj obj{ 5 };
  auto e = obj.get_cb_1();
  auto f = obj.get_cb_2();

  function(a, "fnc a");
  function(b, "fnc b");
  function(c, "obj c");
  function(d, "obj d");
  function(e, "obj e");
  function(f, "obj f");

  return 0;
}
