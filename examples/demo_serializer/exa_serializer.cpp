#include <WLib_serializer.hpp>
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

  template <typename byte_sink_t> static constexpr void serialize(byte_sink_t& sink, type_t const& value, ByteOrder const& byte_order = ByteOrder::native)
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

  template <typename byte_source_t> static constexpr type_t deserialize(byte_source_t& source, ByteOrder const& byte_order = ByteOrder::native)
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
  null_sink_t                sink_a;
  null_source_t              source_a;
  [[maybe_unused]] no_type_t no_sink_a;
  [[maybe_unused]] no_type_t no_source_a;

  static_assert(WLib::is_byte_sink_v<null_sink_t>);
  static_assert(WLib::is_byte_source_v<null_source_t>);

  WLib::serialize(sink_a, static_cast<char>(0xdd));
  WLib::serialize(sink_a, static_cast<int>(0xAABBCCDD));
  WLib::serialize(sink_a, my_type());

  char c;
  int  i;
  c          = WLib::deserialize<char>(source_a);
  c          = WLib::deserialize<char>(source_a);
  i          = WLib::deserialize<int>(source_a);
  my_type mt = WLib::deserialize<my_type>(source_a);

  return 0;
}
