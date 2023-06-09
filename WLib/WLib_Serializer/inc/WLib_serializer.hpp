#pragma once
#ifndef WLIB_SERIALIZER_HPP
#define WLIB_SERIALIZER_HPP

#include <WLib_Byte_Sink_Concept.hpp>
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
      using type_t = typename serializeable_base<T>::type_t;

      template <typename byte_buffer_sink_t>
      static constexpr void serialize(byte_buffer_sink_t& sink, type_t const& value, ByteOrder const& byte_order = ByteOrder::native)
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
      using type_t = typename serializeable_base<T>::type_t;

      template <typename byte_buffer_source_t>
      static constexpr type_t deserialize(byte_buffer_source_t& source, ByteOrder const& byte_order = ByteOrder::native)
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
}    // namespace WLib

#endif    // !WLIB_SERIALIZER_HPP
