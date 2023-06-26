#pragma once
#ifndef WLIB_BYTE_BUFFER_SINK_HPP
#define WLIB_BYTE_BUFFER_SINK_HPP

#include <WLib_Byte_Sink_Concept.hpp>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace WLib
{
  class byte_buffer_sink_t;
  class byte_buffer_source_t;

  class byte_buffer_sink_t
  {
  public:
    template <std::size_t N> constexpr byte_buffer_sink_t(std::byte (&buffer)[N]);
    
    constexpr byte_buffer_sink_t(std::byte* begin, std::byte const* end);
    
    constexpr void operator()(std::byte const& x);

    [[nodiscard]] constexpr std::byte*           begin() const;
    [[nodiscard]] constexpr std::byte const*     cbegin() const;
    [[nodiscard]] constexpr std::byte const*     end() const;
    [[nodiscard]] constexpr std::byte const*     cend() const;

    [[nodiscard]] constexpr std::size_t          get_number_of_used_bytes() const;
    [[nodiscard]] constexpr std::size_t          get_number_of_remaining_bytes() const;
    constexpr void                               adjust_positon(std::ptrdiff_t const& offset);
    constexpr void                               clear();

    [[nodiscard]] constexpr byte_buffer_source_t get_byte_source() const;

  private:
    std::byte*       m_b;
    std::byte const* m_e;
    std::byte*       m_c;
  };

  class byte_buffer_source_t
  {
  public:
    template <std::size_t N> constexpr byte_buffer_source_t(std::byte (&buffer)[N]);

    constexpr byte_buffer_source_t(std::byte* begin, std::byte const* end);

    constexpr std::byte operator()();

    [[nodiscard]] constexpr std::byte*       begin() const;
    [[nodiscard]] constexpr std::byte const* cbegin() const;
    [[nodiscard]] constexpr std::byte const* end() const;
    [[nodiscard]] constexpr std::byte const* cend() const;

    [[nodiscard]] constexpr std::size_t get_number_of_processed_bytes() const;
    [[nodiscard]] constexpr std::size_t get_number_of_remaining_bytes() const;
    constexpr void                      adjust_positon(std::ptrdiff_t const& offset);
    constexpr void                      restart();

  private:
    std::byte*       m_b;
    std::byte const* m_e;
    std::byte*       m_c;
  };

  static_assert(is_byte_sink_v<byte_buffer_sink_t>);
  static_assert(is_byte_source_v<byte_buffer_source_t>);
}    // namespace WLib


template <std::size_t N>
constexpr WLib::byte_buffer_sink_t::byte_buffer_sink_t(std::byte (&buffer)[N])
    : m_b(&buffer[0])
    , m_e(&buffer[N])
    , m_c(&buffer[0])
{
}

constexpr WLib::byte_buffer_sink_t::byte_buffer_sink_t(std::byte* begin, std::byte const* end)
    : m_b(begin)
    , m_e(end)
    , m_c(begin)
{
}

constexpr void WLib::byte_buffer_sink_t::operator()(std::byte const& x)
{
  if (this->m_c >= this->m_e)
    return;

  *this->m_c++ = x;
}

[[nodiscard]] constexpr std::byte* WLib::byte_buffer_sink_t::begin() const { return this->m_b; }

[[nodiscard]] constexpr std::byte const* WLib::byte_buffer_sink_t::cbegin() const { return this->m_b; }

[[nodiscard]] constexpr std::byte const* WLib::byte_buffer_sink_t::end() const { return this->m_c; }

[[nodiscard]] constexpr std::byte const* WLib::byte_buffer_sink_t::cend() const { return this->m_c; }

[[nodiscard]] constexpr std::size_t WLib::byte_buffer_sink_t::get_number_of_used_bytes() const { return this->m_c - this->m_b; }

[[nodiscard]] constexpr std::size_t WLib::byte_buffer_sink_t::get_number_of_remaining_bytes() const { return this->m_e - this->m_c; }

constexpr void WLib::byte_buffer_sink_t::adjust_positon(std::ptrdiff_t const& offset)
{
  std::byte* tmp = this->m_c + offset;
  if (tmp < this->m_b)
  {
    this->m_c = this->m_b;
  }
  else if (tmp > this->m_e)
  {
    this->m_c += this->m_e - this->m_c;
  }
  else
  {
    this->m_c = tmp;
  }
}
constexpr void WLib::byte_buffer_sink_t::clear() { this->m_c = this->m_b; }

[[nodiscard]] constexpr WLib::byte_buffer_source_t WLib::byte_buffer_sink_t::get_byte_source() const { return byte_buffer_source_t{ this->m_b, this->m_c }; }

template <std::size_t N>
constexpr WLib::byte_buffer_source_t::byte_buffer_source_t(std::byte (&buffer)[N])
    : m_b(&buffer[0])
    , m_e(&buffer[N])
    , m_c(&buffer[0])
{
}

constexpr WLib::byte_buffer_source_t::byte_buffer_source_t(std::byte* begin, std::byte const* end)
    : m_b(begin)
    , m_e(end)
    , m_c(begin)
{
}

constexpr std::byte WLib::byte_buffer_source_t::operator()()
{
  if (this->m_c < this->m_e)
  {
    return *this->m_c++;
  }
  return std::byte();
}

[[nodiscard]] constexpr std::byte* WLib::byte_buffer_source_t::begin() const { return this->m_b; }

[[nodiscard]] constexpr std::byte const* WLib::byte_buffer_source_t::cbegin() const { return this->m_b; }

[[nodiscard]] constexpr std::byte const* WLib::byte_buffer_source_t::end() const { return this->m_c; }

[[nodiscard]] constexpr std::byte const* WLib::byte_buffer_source_t::cend() const { return this->m_c; }

[[nodiscard]] constexpr std::size_t WLib::byte_buffer_source_t::get_number_of_processed_bytes() const { return this->m_c - this->m_b; }

[[nodiscard]] constexpr std::size_t WLib::byte_buffer_source_t::get_number_of_remaining_bytes() const { return this->m_e - this->m_c; }

constexpr void WLib::byte_buffer_source_t::adjust_positon(std::ptrdiff_t const& offset)
{
  std::byte* tmp = this->m_c + offset;
  if (tmp < this->m_b)
  {
    this->m_c = this->m_b;
  }
  else if (tmp > this->m_e)
  {
    this->m_c += this->m_e - this->m_c;
  }
  else
  {
    this->m_c = tmp;
  }
}

constexpr void WLib::byte_buffer_source_t::restart() { this->m_c = this->m_b; }

#endif    // !WLIB_BYTE_BUFFER_SINK_HPP
