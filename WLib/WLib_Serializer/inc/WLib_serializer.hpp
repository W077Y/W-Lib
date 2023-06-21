#pragma once
#ifndef WLIB_SERIALIZER_HPP
#define WLIB_SERIALIZER_HPP

#include <cstddef>
#include <type_traits>

namespace WLib
{
  enum class ByteOrder
  {
    native,
    little_endian = native,
    big_endian,
  };

  struct null_sink_t
  {
    void operator()(std::byte const&) {}
  };

  struct null_source_t
  {
    std::byte operator()() { return std::byte(); }
  };

  template <typename T>
  constexpr bool is_byte_sink_v = std::is_same_v<decltype(std::declval<T>()(std::declval<std::byte const&>())), void>;

  template <typename T>
  constexpr bool is_byte_source_v = std::is_same_v<decltype(std::declval<T>()()), std::byte>;

  template <typename byte_sink_t>
  constexpr std::enable_if_t<is_byte_sink_v<byte_sink_t>, void> serialize(byte_sink_t& sink, char const& val, ByteOrder const& byte_order = ByteOrder::native)
  {
  }

  template <typename byte_source_t, typename T>
  [[no_discard]] constexpr std::enable_if_t<is_byte_source_v<byte_source_t>, T> deserialize(byte_source_t&   source,
                                                                                            ByteOrder const& byte_order = ByteOrder::native)
  {
  }

  //template <typename byte_source_t>
  //template <typename T>
  //[[no_discard]] constexpr std::enable_if_t<is_byte_source_v<byte_source_t>, T> deserialize<byte_source_t, T>(byte_source_t&   source,
  //                                                                                          ByteOrder const& byte_order = ByteOrder::native)
  //{
  //}
}    // namespace WLib
#endif
