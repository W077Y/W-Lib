#pragma once
#ifndef WLIB_SERIALIZER_HPP
#define WLIB_SERIALIZER_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace WLib
{
  enum class ByteOrder
  {
    native,
    little_endian = native,
    big_endian,
  };

  template <typename T> constexpr bool is_byte_sink_v   = std::is_same_v<void, decltype(std::declval<T>()(std::declval<std::byte const&>()))>;
  template <typename T> constexpr bool is_byte_source_v = std::is_same_v<std::byte, decltype(std::declval<T>()())>;

  namespace internal
  {
    struct not_seriailizeable_type
    {
      static constexpr bool value = false;
    };
    template <typename T> struct serializeable_base
    {
      using type_t                = std::remove_cv_t<T>;
      static constexpr bool value = true;
    };
    template <typename T> struct native_serializeable_type: public serializeable_base<T>
    {
      using serializeable_base<T>::type_t;

      template <typename byte_buffer_sink_t> static constexpr void serialize(byte_buffer_sink_t& sink, type_t const& value, ByteOrder const& byte_order = ByteOrder::native)
      {
        if (byte_order == ByteOrder::native)
        {
          for (std::size_t i = 0; i < sizeof(type_t);)
          {
            sink(reinterpret_cast<std::byte const*>(&value)[i++]);
          }
        }
        else
        {
          for (std::size_t i = sizeof(type_t); i > 0;)
          {
            sink(reinterpret_cast<std::byte const*>(&value)[--i]);
          }
        }
      }
    };

    template <typename T> constexpr bool is_native_serializable_v = false;

    template <> constexpr bool is_native_serializable_v<std::byte> = true;
    template <> constexpr bool is_native_serializable_v<char>      = true;
    template <> constexpr bool is_native_serializable_v<char16_t>  = true;
    template <> constexpr bool is_native_serializable_v<char32_t>  = true;

    template <> constexpr bool is_native_serializable_v<signed char>      = true;
    template <> constexpr bool is_native_serializable_v<signed short>     = true;
    template <> constexpr bool is_native_serializable_v<signed int>       = true;
    template <> constexpr bool is_native_serializable_v<signed long>      = true;
    template <> constexpr bool is_native_serializable_v<signed long long> = true;

    template <> constexpr bool is_native_serializable_v<unsigned char>      = true;
    template <> constexpr bool is_native_serializable_v<unsigned short>     = true;
    template <> constexpr bool is_native_serializable_v<unsigned int>       = true;
    template <> constexpr bool is_native_serializable_v<unsigned long>      = true;
    template <> constexpr bool is_native_serializable_v<unsigned long long> = true;

    template <> constexpr bool is_native_serializable_v<float>       = true;
    template <> constexpr bool is_native_serializable_v<double>      = true;
    template <> constexpr bool is_native_serializable_v<long double> = true;

    struct not_deserializeable_type
    {
      static constexpr bool value = false;
    };
    template <typename T> struct deserializeable_base
    {
      using type_t                = std::remove_cv_t<T>;
      static constexpr bool value = true;
    };
    template <typename T> struct native_deserializeable_type: public deserializeable_base<T>
    {
      using deserializeable_base<T>::type_t;

      template <typename byte_buffer_source_t> static constexpr type_t deserialize(byte_buffer_source_t& source, ByteOrder const& byte_order = ByteOrder::native)
      {
        type_t ret;
        if (byte_order == ByteOrder::native)
        {
          for (std::size_t i = 0; i < sizeof(type_t);)
          {
            reinterpret_cast<std::byte*>(&ret)[i++] = source();
          }
        }
        else
        {
          for (std::size_t i = sizeof(type_t); i > 0;)
          {
            reinterpret_cast<std::byte*>(&ret)[--i] = source();
          }
        }
        return ret;
      }
    };

    template <typename T> constexpr bool is_native_deserializable_v = false;

    template <> constexpr bool is_native_deserializable_v<std::byte> = true;
    template <> constexpr bool is_native_deserializable_v<char>      = true;
    template <> constexpr bool is_native_deserializable_v<char16_t>  = true;
    template <> constexpr bool is_native_deserializable_v<char32_t>  = true;

    template <> constexpr bool is_native_deserializable_v<signed char>      = true;
    template <> constexpr bool is_native_deserializable_v<signed short>     = true;
    template <> constexpr bool is_native_deserializable_v<signed int>       = true;
    template <> constexpr bool is_native_deserializable_v<signed long>      = true;
    template <> constexpr bool is_native_deserializable_v<signed long long> = true;

    template <> constexpr bool is_native_deserializable_v<unsigned char>      = true;
    template <> constexpr bool is_native_deserializable_v<unsigned short>     = true;
    template <> constexpr bool is_native_deserializable_v<unsigned int>       = true;
    template <> constexpr bool is_native_deserializable_v<unsigned long>      = true;
    template <> constexpr bool is_native_deserializable_v<unsigned long long> = true;

    template <> constexpr bool is_native_deserializable_v<float>       = true;
    template <> constexpr bool is_native_deserializable_v<double>      = true;
    template <> constexpr bool is_native_deserializable_v<long double> = true;
  }    // namespace internal

  template <typename T>
  struct serializer_traits
      : public std::conditional_t<internal::is_native_serializable_v<T>, internal::native_serializeable_type<T>, internal::not_seriailizeable_type>
  {
  };

  template <typename T> constexpr bool is_serializeable_v = serializer_traits<T>::value;

  template <typename T, typename sink_t>
  constexpr std::enable_if_t<is_serializeable_v<T> && is_byte_sink_v<sink_t>, void>
  serialize(sink_t& sink, T const& value, ByteOrder const& byte_order = ByteOrder::native)
  {
    return serializer_traits<T>::serialize(sink, value, byte_order);
  }

  template <typename T>
  struct deserializer_traits
      : public std::conditional_t<internal::is_native_deserializable_v<T>, internal::native_deserializeable_type<T>, internal::not_deserializeable_type>
  {
  };

  template <typename T> constexpr bool is_deserializeable_v = deserializer_traits<T>::value;

  template <typename T, typename source_t>
  constexpr std::enable_if_t<is_deserializeable_v<T> && is_byte_source_v<source_t>, T> deserialize(source_t&        source,
                                                                                                   ByteOrder const& byte_order = ByteOrder::native)
  {
    return deserializer_traits<T>::deserialize(source, byte_order);
  }

  class byte_buffer_source_t
  {
  public:
    template <std::size_t N>
    constexpr byte_buffer_source_t(std::byte (&buffer)[N])
        : m_b(&buffer[0])
        , m_e(&buffer[N])
        , m_c(&buffer[0])
    {
    }

    constexpr byte_buffer_source_t(std::byte* begin, std::byte const* end)
        : m_b(begin)
        , m_e(end)
        , m_c(begin)
    {
    }

    constexpr std::byte operator()()
    {
      if (this->m_c < this->m_e)
      {
        return *this->m_c++;
      }
      return std::byte();
    }

    [[nodiscard]] constexpr std::byte*       begin() const { return this->m_b; }
    [[nodiscard]] constexpr std::byte const* cbegin() const { return this->m_b; }
    [[nodiscard]] constexpr std::byte const* end() const { return this->m_c; }
    [[nodiscard]] constexpr std::byte const* cend() const { return this->m_c; }

    [[nodiscard]] constexpr std::size_t get_number_of_processed_bytes() const { return this->m_c - this->m_b; }
    [[nodiscard]] constexpr std::size_t get_number_of_remaining_bytes() const { return this->m_e - this->m_c; }
    constexpr void                      restart() { this->m_c = this->m_b; }

  private:
    std::byte*       m_b;
    std::byte const* m_e;
    std::byte*       m_c;
  };

  class byte_buffer_sink_t
  {
  public:
    template <std::size_t N>
    constexpr byte_buffer_sink_t(std::byte (&buffer)[N])
        : m_b(&buffer[0])
        , m_e(&buffer[N])
        , m_c(&buffer[0])
    {
    }

    constexpr byte_buffer_sink_t(std::byte* begin, std::byte const* end)
        : m_b(begin)
        , m_e(end)
        , m_c(begin)
    {
    }

    constexpr void operator()(std::byte const& x)
    {
      if (this->m_c >= this->m_e)
        return;

      *this->m_c++ = x;
    }

    [[nodiscard]] constexpr std::byte*       begin() const { return this->m_b; }
    [[nodiscard]] constexpr std::byte const* cbegin() const { return this->m_b; }
    [[nodiscard]] constexpr std::byte const* end() const { return this->m_c; }
    [[nodiscard]] constexpr std::byte const* cend() const { return this->m_c; }

    [[nodiscard]] constexpr std::size_t get_number_of_used_bytes() const { return this->m_c - this->m_b; }
    [[nodiscard]] constexpr std::size_t get_number_of_remaining_bytes() const { return this->m_e - this->m_c; }
    void                                clear() { this->m_c = this->m_b; }

    [[nodiscard]] constexpr byte_buffer_source_t get_byte_source() const { return byte_buffer_source_t{ this->m_b, this->m_c }; }

  private:
    std::byte*       m_b;
    std::byte const* m_e;
    std::byte*       m_c;
  };

}    // namespace WLib

#endif    // !WLIB_SERIALIZER_HPP
