#pragma once
#ifndef WLIB_SERIALIZER_HPP
#define WLIB_SERIALIZER_HPP

#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <type_traits>

namespace wlib
{
  struct reverse_t
  {
    explicit reverse_t() = default;
  };

  inline constexpr reverse_t reverse{};

  class ByteSink_Interface
  {
  public:
    virtual ~ByteSink_Interface() = default;

    virtual bool operator()(std::byte const& value) = 0;

    virtual std::size_t operator()(std::byte const* begin, std::byte const* end)
    {
      std::size_t len = 0;
      while (begin < end)
      {
        if (!this->operator()(*begin++))
          return len;
        ++len;
      }
      return len;
    }
    virtual std::size_t operator()(reverse_t, std::byte const* begin, std::byte const* end)
    {
      std::size_t len = 0;
      while (begin < end)
      {
        if (!this->operator()(*--end))
          return len;
        ++len;
      }
      return len;
    }

    virtual std::size_t operator()(std::byte const* ptr, std::size_t const& len) { return this->operator()(ptr, ptr + len); }
    virtual std::size_t operator()(reverse_t, std::byte const* ptr, std::size_t const& len) { return this->operator()(reverse, ptr, ptr + len); }

    virtual std::size_t operator()(std::span<std::byte> const& span) { return this->operator()(span.data(), span.size()); }
    virtual std::size_t operator()(reverse_t, std::span<std::byte> const& span) { return this->operator()(reverse, span.data(), span.size()); }
  };

  class ByteSource_Interface
  {
  public:
    virtual ~ByteSource_Interface() = default;

    virtual bool operator()(std::byte& value) = 0;

    virtual std::size_t operator()(std::byte* begin, std::byte const* end)
    {
      std::size_t len = 0;
      for (std::byte* cur = begin; cur < end;)
      {
        if (!this->operator()(*cur++))
          return len;
        ++len;
      }
      return len;
    }
    virtual std::size_t operator()(reverse_t, std::byte* begin, std::byte const* end)
    {
      std::size_t len = 0;
      for (std::byte* cur = begin + (end - begin); begin < cur;)
      {
        if (!this->operator()(*--cur))
          return len;
        ++len;
      }
      return len;
    }

    virtual std::size_t operator()(std::byte* ptr, std::size_t const& len) { return this->operator()(ptr, ptr + len); }
    virtual std::size_t operator()(reverse_t, std::byte* ptr, std::size_t const& len) { return this->operator()(reverse, ptr, ptr + len); }

    virtual std::size_t operator()(std::span<std::byte> const& span) { return this->operator()(span.data(), span.size()); }
    virtual std::size_t operator()(reverse_t, std::span<std::byte> const& span) { return this->operator()(reverse, span.data(), span.size()); }
  };

  class ByteSink_ExBuffer;
  class ByteSource_ExBuffer;

  class ByteSink_ExBuffer: public ByteSink_Interface
  {
  public:
    constexpr ByteSink_ExBuffer(std::byte* begin, std::byte const* end)
        : m_b(begin)
        , m_e(end)
        , m_c(begin)
    {
    }
    constexpr ByteSink_ExBuffer(std::byte* ptr, std::size_t const& len)
        : m_b(ptr)
        , m_e(ptr + len)
        , m_c(ptr)
    {
    }
    constexpr ByteSink_ExBuffer(std::span<std::byte>& span)
        : ByteSink_ExBuffer(span.data(), span.size())
    {
    }
    template <std::size_t N>
    constexpr ByteSink_ExBuffer(std::byte (&buffer)[N])
        : m_b(&buffer[0])
        , m_e(&buffer[N])
        , m_c(&buffer[0])
    {
    }

    using ByteSink_Interface::operator();

    virtual constexpr bool operator()(std::byte const& value) override
    {
      if (this->m_c >= this->m_e)
        return false;

      *this->m_c++ = value;
      return true;
    }

    [[nodiscard]] constexpr std::byte*       begin() const { return this->m_b; }
    [[nodiscard]] constexpr std::byte const* cbegin() const { return this->m_b; }
    [[nodiscard]] constexpr std::byte const* end() const { return this->m_c; }
    [[nodiscard]] constexpr std::byte const* cend() const { return this->m_c; }

    [[nodiscard]] constexpr std::size_t get_number_of_used_bytes() const { return this->m_c - this->m_b; }
    [[nodiscard]] constexpr std::size_t get_number_of_remaining_bytes() const { return this->m_e - this->m_c; }
    [[nodiscard]] constexpr std::size_t get_total_number_of_bytes() const { return this->m_e - this->m_b; }

    constexpr void adjust_position(std::ptrdiff_t const& offset)
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
    constexpr void clear() { this->m_c = this->m_b; }

    [[nodiscard]] constexpr ByteSource_ExBuffer get_bytesource() const;

  private:
    std::byte*       m_b;
    std::byte const* m_e;
    std::byte*       m_c;
  };

  class ByteSource_ExBuffer: public ByteSource_Interface
  {
  public:
    constexpr ByteSource_ExBuffer(std::byte const* begin, std::byte const* end)
        : m_b(begin)
        , m_e(end)
        , m_c(begin)
    {
    }
    constexpr ByteSource_ExBuffer(std::byte const* ptr, std::size_t const& len)
        : m_b(ptr)
        , m_e(ptr + len)
        , m_c(ptr)
    {
    }
    constexpr ByteSource_ExBuffer(std::span<std::byte const>& span)
        : ByteSource_ExBuffer(span.data(), span.size())
    {
    }
    template <std::size_t N>
    constexpr ByteSource_ExBuffer(std::byte const (&buffer)[N])
        : m_b(&buffer[0])
        , m_e(&buffer[N])
        , m_c(&buffer[0])
    {
    }

    using ByteSource_Interface::operator();
    constexpr bool operator()(std::byte& value)
    {
      if (this->m_c < this->m_e)
      {
        value = *this->m_c++;
        return true;
      }
      return false;
    }

    [[nodiscard]] constexpr std::byte const* cbegin() const { return this->m_b; }
    [[nodiscard]] constexpr std::byte const* cend() const { return this->m_c; }

    [[nodiscard]] constexpr std::size_t get_number_of_processed_bytes() const { return this->m_c - this->m_b; }
    [[nodiscard]] constexpr std::size_t get_number_of_remaining_bytes() const { return this->m_e - this->m_c; }
    [[nodiscard]] constexpr std::size_t get_total_number_of_bytes() const { return this->m_e - this->m_b; }

    constexpr void adjust_position(std::ptrdiff_t const& offset)
    {
      std::byte const* tmp = this->m_c + offset;
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
    constexpr void restart() { this->m_c = this->m_b; }

  private:
    std::byte const* m_b;
    std::byte const* m_e;
    std::byte const* m_c;
  };

  template <std::size_t N> class ByteSink: public ByteSink_Interface
  {
  public:
    ByteSink() = default;

    using ByteSink_Interface::operator();

    virtual constexpr bool operator()(std::byte const& value) override
    {
      if (this->m_c >= (this->m_data + N))
        return false;

      *this->m_c++ = value;
      return true;
    }

    [[nodiscard]] constexpr std::byte*       begin() const { return this->m_data; }
    [[nodiscard]] constexpr std::byte const* cbegin() const { return this->m_data; }
    [[nodiscard]] constexpr std::byte const* end() const { return this->m_c; }
    [[nodiscard]] constexpr std::byte const* cend() const { return this->m_c; }

    [[nodiscard]] constexpr std::size_t get_number_of_used_bytes() const { return this->m_c - this->m_data; }
    [[nodiscard]] constexpr std::size_t get_number_of_remaining_bytes() const { return (this->m_data + N) - this->m_c; }
    [[nodiscard]] constexpr std::size_t get_total_number_of_bytes() const { return N; }

    constexpr void adjust_position(std::ptrdiff_t const& offset)
    {
      std::byte* tmp = this->m_c + offset;
      if (tmp < this->m_data)
      {
        this->m_c = this->m_data;
      }
      else if (tmp > (this->m_data + N))
      {
        this->m_c += (this->m_data + N) - this->m_c;
      }
      else
      {
        this->m_c = tmp;
      }
    }
    constexpr void clear() { this->m_c = this->m_data; }

    [[nodiscard]] constexpr ByteSource_ExBuffer get_bytesource() const { return ByteSource_ExBuffer{ this->m_data, this->m_c }; }

  private:
    std::byte  m_data[N]{};
    std::byte* m_c = this->m_data;
  };

  constexpr ByteSource_ExBuffer ByteSink_ExBuffer::get_bytesource() const { return ByteSource_ExBuffer{ this->m_b, this->m_c }; }

  namespace internal
  {
    template <typename T> constexpr bool native_serialize_able = false;

    template <> constexpr bool native_serialize_able<std::byte>          = true;
    template <> constexpr bool native_serialize_able<bool>               = true;
    template <> constexpr bool native_serialize_able<char>               = true;
    template <> constexpr bool native_serialize_able<signed char>        = true;
    template <> constexpr bool native_serialize_able<unsigned char>      = true;
    template <> constexpr bool native_serialize_able<char16_t>           = true;
    template <> constexpr bool native_serialize_able<char32_t>           = true;
    template <> constexpr bool native_serialize_able<signed short>       = true;
    template <> constexpr bool native_serialize_able<unsigned short>     = true;
    template <> constexpr bool native_serialize_able<signed int>         = true;
    template <> constexpr bool native_serialize_able<unsigned int>       = true;
    template <> constexpr bool native_serialize_able<signed long>        = true;
    template <> constexpr bool native_serialize_able<unsigned long>      = true;
    template <> constexpr bool native_serialize_able<signed long long>   = true;
    template <> constexpr bool native_serialize_able<unsigned long long> = true;
    template <> constexpr bool native_serialize_able<float>              = true;
    template <> constexpr bool native_serialize_able<double>             = true;
    template <> constexpr bool native_serialize_able<long double>        = true;
  }    // namespace internal

  template <typename T>
  requires(internal::native_serialize_able<T>) auto serialize(wlib::ByteSink_Interface& sink, T const& value, std::endian const& endian = std::endian::native)
  {
    std::byte const* begin = reinterpret_cast<std::byte const*>(&value);
    std::byte const* end   = begin + sizeof(T);
    if (endian == std::endian::native)
      return sink(begin, end);
    else
      return sink(reverse, begin, end);
  }

  template <typename T> std::optional<T> deserialize(wlib::ByteSource_Interface& source, std::endian const& endian = std::endian::native);

  template <typename T>
  requires(internal::native_serialize_able<T>) std::optional<T> deserialize(wlib::ByteSource_Interface& source, std::endian const& endian = std::endian::native)
  {
    T                ret;
    std::byte*       begin = reinterpret_cast<std::byte*>(&ret);
    std::byte const* end   = begin + sizeof(T);
    if (endian == std::endian::native)
    {
      if (source(begin, end) != sizeof(T))
        return std::nullopt;
    }
    else
    {
      if (source(reverse, begin, end) != sizeof(T))
        return std::nullopt;
    }
    return ret;
  }
}    // namespace wlib

#endif    // !WLIB_SERIALIZER_HPP
