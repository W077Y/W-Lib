#include <WLib_Byte_Buffer_Sink.hpp>
#include <ut_catch.hpp>

static_assert(WLib::is_byte_sink_v<WLib::byte_buffer_sink_t>);
static_assert(WLib::is_byte_source_v<WLib::byte_buffer_source_t>);

TEST_CASE()
{
  std::byte buffer[100];
  {
    WLib::byte_buffer_sink_t sink{ buffer };

    for (int i = 0; i < 110; i++)
      sink(static_cast<std::byte>(i + 3));
  }
  {
    for (int i = 0; i < 100; i++)
      REQUIRE(buffer[i] == static_cast<std::byte>(i + 3));
  }
}

TEST_CASE()
{
  std::byte buffer[100];
  {
    for (int i = 0; i < 100; i++)
      buffer[i] = static_cast<std::byte>(i + 3);
  }
  {
    WLib::byte_buffer_source_t source{ buffer };

    for (int i = 0; i < 100; i++)
      REQUIRE(source() == static_cast<std::byte>(i + 3));
  }
}

TEST_CASE()
{
  std::byte buffer[100];

  {
    WLib::byte_buffer_sink_t sink{ buffer };
    REQUIRE(sink.get_number_of_remaining_bytes() == 100);
    REQUIRE(sink.get_number_of_used_bytes() == 0);

    for (int i = 0; i < 10; i++)
      sink(static_cast<std::byte>(i + 3));

    {
      WLib::byte_buffer_source_t source = sink.get_byte_source();
      REQUIRE(source.get_number_of_processed_bytes() == 0);
      REQUIRE(source.get_number_of_remaining_bytes() == 10);
    }

    for (int i = 10; i < 100; i++)
      sink(static_cast<std::byte>(i + 3));

    {
      WLib::byte_buffer_source_t source = sink.get_byte_source();
      REQUIRE(source.get_number_of_processed_bytes() == 0);
      REQUIRE(source.get_number_of_remaining_bytes() == 100);
    }

    REQUIRE(sink.get_number_of_remaining_bytes() == 0);
    REQUIRE(sink.get_number_of_used_bytes() == 100);

    sink.clear();
    REQUIRE(sink.get_number_of_remaining_bytes() == 100);
    REQUIRE(sink.get_number_of_used_bytes() == 0);

    for (int i = 0; i < 10; i++)
      sink(static_cast<std::byte>(i + 13));

    REQUIRE(sink.get_number_of_remaining_bytes() == 90);
    REQUIRE(sink.get_number_of_used_bytes() == 10);

    sink.adjust_positon(10);
    REQUIRE(sink.get_number_of_remaining_bytes() == 80);
    REQUIRE(sink.get_number_of_used_bytes() == 20);

    for (int i = 20; i < 100; i++)
      sink(static_cast<std::byte>(i + 23));
    REQUIRE(sink.get_number_of_remaining_bytes() == 0);
    REQUIRE(sink.get_number_of_used_bytes() == 100);

    sink.adjust_positon(-10);
    REQUIRE(sink.get_number_of_remaining_bytes() == 10);
    REQUIRE(sink.get_number_of_used_bytes() == 90);

    for (int i = 90; i < 100; i++)
      sink(static_cast<std::byte>(i + 33));
  }
  {
    for (int i = 0; i < 10; i++)
      REQUIRE(buffer[i] == static_cast<std::byte>(i + 13));
    for (int i = 10; i < 20; i++)
      REQUIRE(buffer[i] == static_cast<std::byte>(i + 3));
    for (int i = 20; i < 90; i++)
      REQUIRE(buffer[i] == static_cast<std::byte>(i + 23));
    for (int i = 90; i < 100; i++)
      REQUIRE(buffer[i] == static_cast<std::byte>(i + 33));
  }
}

TEST_CASE()
{
  std::byte buffer[100];
  {
    for (int i = 0; i < 100; i++)
      buffer[i] = static_cast<std::byte>(i + 3);
  }
  {
    WLib::byte_buffer_source_t source{ buffer };
    REQUIRE(source.get_number_of_processed_bytes() == 0);
    REQUIRE(source.get_number_of_remaining_bytes() == 100);

    for (int i = 0; i < 100; i++)
      REQUIRE(source() == static_cast<std::byte>(i + 3));

    REQUIRE(source.get_number_of_processed_bytes() == 100);
    REQUIRE(source.get_number_of_remaining_bytes() == 0);

    source.restart();
    REQUIRE(source.get_number_of_processed_bytes() == 0);
    REQUIRE(source.get_number_of_remaining_bytes() == 100);

    source.adjust_positon(10);
    REQUIRE(source.get_number_of_processed_bytes() == 10);
    REQUIRE(source.get_number_of_remaining_bytes() == 90);

    source.adjust_positon(100);
    REQUIRE(source.get_number_of_processed_bytes() == 100);
    REQUIRE(source.get_number_of_remaining_bytes() == 0);

    source.adjust_positon(-200);
    REQUIRE(source.get_number_of_processed_bytes() == 0);
    REQUIRE(source.get_number_of_remaining_bytes() == 100);
  }
}

TEST_CASE()
{
  std::byte buffer[100] = {};
  {
    WLib::byte_buffer_sink_t sink{ buffer };

    sink.adjust_positon(10);
    REQUIRE(sink.get_number_of_used_bytes() == 10);
    REQUIRE(sink.get_number_of_remaining_bytes() == 90);

    for (auto& obj : sink)
    {
      obj = static_cast<std::byte>(0xAA);
    }
  }
  {
    for (int i = 0; i < 10; i++)
      REQUIRE(buffer[i] == static_cast<std::byte>(0xAA));

    for (int i = 10; i < 100; i++)
      REQUIRE(buffer[i] == static_cast<std::byte>(0x00));
  }
}

TEST_CASE()
{
  std::byte buffer[100];
  {
    for (int i = 0; i < 100; i++)
      buffer[i] = static_cast<std::byte>(i + 3);
  }

  {
    WLib::byte_buffer_source_t source{ buffer };

    int idx = 0;
    for (auto const& el : source)
    {
      REQUIRE(static_cast<std::byte>(idx + 3) == el);
      ++idx;
    }
  }
}
