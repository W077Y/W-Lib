#pragma once
#ifndef WLIB_CONCEPTS_HPP
#define WLIB_CONCEPTS_HPP

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace WLib::Concepts
{

  template <typename T> concept Byte_Sink = requires(T & sink, std::byte const& value)
  {
    {
      sink(value)
    } -> std::same_as<void>;
  };

  template <typename T> concept Byte_Source = requires(T & source)
  {
    {
      source()
    } -> std::same_as<std::byte>;
  };

  template <typename T> concept CRC_Calculator = requires(T calculator, T const& const_calculator, std::byte const* beg, std::byte const* end, std::size_t const& len)
  {
    typename T::value_type;
    {
      calculator.reset()
    } noexcept -> std::same_as<void>;
    {
      calculator(beg, end)
    } noexcept -> std::same_as<typename T::value_type>;
    {
      calculator(beg, len)
    } noexcept -> std::same_as<typename T::value_type>;
    {
      const_calculator()
    } noexcept -> std::same_as<typename T::value_type>;
  };

}    // namespace WLib::Concepts

#endif    // !WLIB_SERIALIZER_HPP
