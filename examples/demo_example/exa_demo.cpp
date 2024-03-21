#include <iostream>

class pimpl_ex
{
public:
  pimpl_ex();
  ~pimpl_ex();

  int aaa() const;

private:
  class impl_t;

  impl_t& m_obj;
};

int main()
{

  auto aa = pimpl_ex();

  std::cout << "This is a demo example! " << aa.aaa() << std::endl;
  return 0;
}

class pimpl_ex::impl_t
{
public:
  impl_t() = default;

  int aaa() const { return this->m_a; }

private:
  int m_a = 1;
};

pimpl_ex::pimpl_ex()
    : m_obj(*new pimpl_ex::impl_t())
{
}
pimpl_ex::~pimpl_ex() { delete &this->m_obj; }

int pimpl_ex::aaa() const { return this->m_obj.aaa(); }
