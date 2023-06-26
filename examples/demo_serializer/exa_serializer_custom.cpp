#include "exa_serializer.hpp"
#include "exa_my_type.hpp"

#include <WLib_Byte_Buffer_Sink.hpp>
#include <WLib_Serializer.hpp>
#include <cassert>

template <> struct WLib::serializer_traits<my_type>
{
  using type_t                = my_type;
  static constexpr bool value = true;

  template <typename byte_buffer_sink_t>
  static constexpr void serialize(byte_buffer_sink_t& sink, type_t const& value, ByteOrder const& byte_order = ByteOrder::native)
  {
    WLib::serialize(sink, value.m_a, byte_order);
    WLib::serialize(sink, value.m_b, byte_order);
    WLib::serialize(sink, value.m_c, byte_order);
    WLib::serialize(sink, value.m_d, byte_order);
    return;
  }
};

template <> struct WLib::deserializer_traits<my_type>
{
  using type_t                = my_type;
  static constexpr bool value = true;

  template <typename byte_buffer_source_t> static constexpr type_t deserialize(byte_buffer_source_t& source, ByteOrder const& byte_order = ByteOrder::native)
  {
    char     a = WLib::deserialize<char>(source, byte_order);
    uint32_t b = WLib::deserialize<uint32_t>(source, byte_order);
    uint64_t c = WLib::deserialize<uint64_t>(source, byte_order);
    char     d = WLib::deserialize<char>(source, byte_order);
    return my_type{ a, b, c, d };
  }
};

int example_de_serializer_custom_type()
{
  std::byte buffer[128] = {};

  uint64_t val = 0;
  {
    WLib::byte_buffer_sink_t sink{ buffer };
    assert(sink.get_number_of_remaining_bytes() == 128);

    my_type tmp = { 1, 2, 3, 4 };
    val         = tmp.get_value();

    WLib::serialize(sink, tmp);
    assert(sink.get_number_of_remaining_bytes() == 114);
  }
  {
    WLib::byte_buffer_source_t source{ buffer };
    assert(source.get_number_of_processed_bytes() == 0);

    my_type tmp = WLib::deserialize<my_type>(source);
    assert(source.get_number_of_processed_bytes() == 14);

    assert(val == tmp.get_value());
  }

  return 0;
}
