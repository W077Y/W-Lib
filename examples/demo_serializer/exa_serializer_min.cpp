#include "exa_serializer.hpp"

#include <WLib_Byte_Buffer_Sink.hpp>
#include <WLib_Serializer.hpp>
#include <cassert>

int example_serializer_min()
{
  std::byte tmp[128] = {};

  {
    WLib::byte_buffer_sink_t sink{ tmp };
    assert(sink.get_number_of_remaining_bytes() == 128);

    int32_t value = 0x42;
    WLib::serialize(sink, value, WLib::ByteOrder::big_endian);

    assert(sink.get_number_of_used_bytes() == 4);
    assert(sink.get_number_of_remaining_bytes() == 124);
  }

  assert(tmp[0] == static_cast<std::byte>(0x00));
  assert(tmp[1] == static_cast<std::byte>(0x00));
  assert(tmp[2] == static_cast<std::byte>(0x00));
  assert(tmp[3] == static_cast<std::byte>(0x42));

  return 0;
}

int example_deserializer_min()
{
  std::byte tmp[128] = {
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x42),
  };

  {
    WLib::byte_buffer_source_t sink{ tmp };
    assert(sink.get_number_of_remaining_bytes() == 128);
    int32_t value = WLib::deserialize<int32_t>(sink, WLib::ByteOrder::big_endian);
    assert(value == 0x42);
    assert(sink.get_number_of_processed_bytes() == 4);
    assert(sink.get_number_of_remaining_bytes() == 124);
  }

  return 0;
}

int example_deserializer_min_2()
{
  std::byte tmp[128];

  {
    WLib::byte_buffer_sink_t sink{ tmp };
    assert(sink.get_number_of_remaining_bytes() == 128);
    assert(sink.get_number_of_used_bytes() == 0);

    int32_t value = 0x42;
    WLib::serialize(sink, value, WLib::ByteOrder::big_endian);
    WLib::serialize(sink, value, WLib::ByteOrder::big_endian);
    assert(sink.get_number_of_used_bytes() == 8);

    auto source = sink.get_byte_source();
    assert(source.get_number_of_remaining_bytes() == 8);
    assert(source.get_number_of_processed_bytes() == 0);

    int32_t value_1 = WLib::deserialize<int32_t>(source, WLib::ByteOrder::big_endian);
    int32_t value_2 = WLib::deserialize<int32_t>(source, WLib::ByteOrder::big_endian);

    assert(source.get_number_of_remaining_bytes() == 0);
    assert(source.get_number_of_processed_bytes() == 8);
    assert(value_1 == 0x42);
    assert(value_2 == 0x42);

    source.restart();
    assert(source.get_number_of_remaining_bytes() == 8);
    assert(source.get_number_of_processed_bytes() == 0);
    int32_t value_3 = WLib::deserialize<int32_t>(source, WLib::ByteOrder::big_endian);
    int32_t value_4 = WLib::deserialize<int32_t>(source, WLib::ByteOrder::big_endian);
    assert(source.get_number_of_remaining_bytes() == 0);
    assert(source.get_number_of_processed_bytes() == 8);
    assert(value_3 == 0x42);
    assert(value_4 == 0x42);

    sink.clear();
    assert(sink.get_number_of_remaining_bytes() == 128);
    assert(sink.get_number_of_used_bytes() == 0);
  }
  return 0;
}
