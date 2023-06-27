#pragma once
#ifndef WLIB_BYTE_SINK_CONCEPT_HPP
#define WLIB_BYTE_SINK_CONCEPT_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace WLib
{
  template <typename T> constexpr bool is_byte_sink_v   = std::is_same_v<void, decltype(std::declval<T>()(std::declval<std::byte const&>()))>;
  template <typename T> constexpr bool is_byte_source_v = std::is_same_v<std::byte, decltype(std::declval<T>()())>;
}    // namespace WLib

#endif    // !WLIB_SERIALIZER_HPP
