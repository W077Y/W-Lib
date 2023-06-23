#include "exa_serializer_min.hpp"

#include <WLib_serializer.hpp>
#include <cassert>

namespace WLib
{
  class byte_source_t
  {
  public:
    template <std::size_t N>
    constexpr byte_source_t(std::byte (&buffer)[N])
        : m_b(&buffer[0])
        , m_e(&buffer[N])
        , m_c(&buffer[0])
    {
    }

    constexpr byte_source_t(std::byte* begin, std::byte const* end)
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

  class byte_sink_t
  {
  public:
    template <std::size_t N>
    constexpr byte_sink_t(std::byte (&buffer)[N])
        : m_b(&buffer[0])
        , m_e(&buffer[N])
        , m_c(&buffer[0])
    {
    }

    constexpr byte_sink_t(std::byte* begin, std::byte const* end)
        : m_b(begin)
        , m_e(end)
        , m_c(begin)
    {
    }

    constexpr bool operator()(std::byte const& x)
    {
      if (this->m_c >= this->m_e)
        return false;

      *this->m_c++ = x;
      return true;
    }

    [[nodiscard]] constexpr std::byte*       begin() const { return this->m_b; }
    [[nodiscard]] constexpr std::byte const* cbegin() const { return this->m_b; }
    [[nodiscard]] constexpr std::byte const* end() const { return this->m_c; }
    [[nodiscard]] constexpr std::byte const* cend() const { return this->m_c; }

    [[nodiscard]] constexpr std::size_t get_number_of_used_bytes() const { return this->m_c - this->m_b; }
    [[nodiscard]] constexpr std::size_t get_number_of_remaining_bytes() const { return this->m_e - this->m_c; }
    void                                clear() { this->m_c = this->m_b; }

    [[nodiscard]] constexpr byte_source_t get_byte_source() const { return byte_source_t{ this->m_b, this->m_e }; }

  private:
    std::byte*       m_b;
    std::byte const* m_e;
    std::byte*       m_c;
  };

}    // namespace WLib

int example_serializer_min()
{
  std::byte tmp[128] = {};

  {
    WLib::byte_sink_t sink{ tmp };
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
    WLib::byte_source_t sink{ tmp };
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
    WLib::byte_sink_t sink{ tmp };
    assert(sink.get_number_of_remaining_bytes() == 128);
    assert(sink.get_number_of_used_bytes() == 0);

    int32_t value = 0x42;
    WLib::serialize(sink, value, WLib::ByteOrder::big_endian);
    WLib::serialize(sink, value, WLib::ByteOrder::big_endian);
    assert(sink.get_number_of_used_bytes() == 8);

    auto    source  = sink.get_byte_source();
    assert(source.get_number_of_remaining_bytes() == 8);
    assert(source.get_number_of_processed_bytes() == 0);

    int32_t value_1 = WLib::deserialize<int32_t>(source, WLib::ByteOrder::big_endian);
    int32_t value_2 = WLib::deserialize<int32_t>(source, WLib::ByteOrder::big_endian);

    int     aaa;
    auto aaa = WLib::deserialize<int32_t>(source, aaa, WLib::ByteOrder::big_endian);
    int32_t val_3 = WLib::deserialize<int32_t>(source, aaa, WLib::ByteOrder::big_endian);
    
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
