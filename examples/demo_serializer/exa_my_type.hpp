#pragma once
#ifndef EXA_MY_TYPE_HPP
#define EXA_MY_TYPE_HPP

#include <cstdint>

struct my_type
{
public:
  constexpr my_type(char a, uint32_t b, uint64_t c, char d)
      : m_a(a)
      , m_b(b)
      , m_c(c)
      , m_d(d)
  {
  }

  constexpr uint64_t get_value() const
  {
    return static_cast<uint64_t>(this->m_a) + static_cast<uint64_t>(this->m_b) + static_cast<uint64_t>(this->m_c) + static_cast<uint64_t>(this->m_d);
  }

  char     m_a;
  uint32_t m_b;
  uint64_t m_c;
  char     m_d;
};

#endif    // !1
