#include "exa_serializer.hpp"
#include <WLib_serializer.hpp>
#include <WLib_Byte_Buffer_Sink.hpp>
#include <iostream>

struct null_sink_t
{
  void operator()(std::byte const&) { return; }
};

struct null_source_t
{
  std::byte operator()() { return std::byte(); }
};

struct no_type_t
{
};

struct my_type
{
  char a;
  int  b;
  long c;
  char d;
};

template <> struct WLib::serializer_traits<my_type>
{
  using type_t                = my_type;
  static constexpr bool value = true;

  template <typename byte_buffer_sink_t> static constexpr void serialize(byte_buffer_sink_t& sink, type_t const& value, ByteOrder const& byte_order = ByteOrder::native)
  {
    WLib::serialize(sink, value.a, byte_order);
    WLib::serialize(sink, value.b, byte_order);
    WLib::serialize(sink, value.c, byte_order);
    WLib::serialize(sink, value.d, byte_order);
    return;
  }
};
template <> struct WLib::deserializer_traits<my_type>
{
  using type_t                = my_type;
  static constexpr bool value = true;

  template <typename byte_buffer_source_t> static constexpr type_t deserialize(byte_buffer_source_t& source, ByteOrder const& byte_order = ByteOrder::native)
  {
    type_t ret;
    ret.a = WLib::deserialize<char>(source, byte_order);
    ret.b = WLib::deserialize<int>(source, byte_order);
    ret.c = WLib::deserialize<long>(source, byte_order);
    ret.d = WLib::deserialize<char>(source, byte_order);
    return ret;
  }
};








int main()
{
  int ret = 0;
  ret |= example_serializer_min();
  ret |= example_deserializer_min();
  ret |= example_deserializer_min_2();
  return ret;
}
