#include <WLib_Byte_Buffer_Sink.hpp>
#include <WLib_serializer.hpp>
#include <ut_catch.hpp>

constexpr std::byte          ref_value_byte         = static_cast<std::byte>(0x01);
constexpr char               ref_value_char         = 0x02;
constexpr char16_t           ref_value_char16_t     = 0x03;
constexpr char32_t           ref_value_char32_t     = 0x04;
constexpr signed char        ref_value_s_char       = 0x05;
constexpr signed short       ref_value_s_short      = 0x06;
constexpr signed int         ref_value_s_int        = 0x07;
constexpr signed long        ref_value_s_long       = 0x08;
constexpr signed long long   ref_value_s_long_long  = 0x09;
constexpr unsigned char      ref_value_us_char      = 0x0A;
constexpr unsigned short     ref_value_us_short     = 0x0B;
constexpr unsigned int       ref_value_us_int       = 0x0C;
constexpr unsigned long      ref_value_us_long      = 0x0D;
constexpr unsigned long long ref_value_us_long_long = 0x0E;
constexpr float              ref_value_float        = 0x0F;
constexpr double             ref_value_double       = 0x10;
constexpr long double        ref_value_long_double  = 0x11;

TEST_CASE()
{
  std::byte buffer[1024];
  for (WLib::ByteOrder byte_order : { WLib::ByteOrder::native, WLib::ByteOrder::little_endian, WLib::ByteOrder::big_endian })
  {
    {
      WLib::byte_buffer_sink_t sink{ buffer };

      REQUIRE(sink.get_number_of_remaining_bytes() == 1024);

      WLib::serialize(sink, ref_value_byte, byte_order);
      WLib::serialize(sink, ref_value_char, byte_order);
      WLib::serialize(sink, ref_value_char16_t, byte_order);
      WLib::serialize(sink, ref_value_char32_t, byte_order);

      WLib::serialize(sink, ref_value_s_char, byte_order);
      WLib::serialize(sink, ref_value_s_short, byte_order);
      WLib::serialize(sink, ref_value_s_int, byte_order);
      WLib::serialize(sink, ref_value_s_long, byte_order);
      WLib::serialize(sink, ref_value_s_long_long, byte_order);

      WLib::serialize(sink, ref_value_us_char, byte_order);
      WLib::serialize(sink, ref_value_us_short, byte_order);
      WLib::serialize(sink, ref_value_us_int, byte_order);
      WLib::serialize(sink, ref_value_us_long, byte_order);
      WLib::serialize(sink, ref_value_us_long_long, byte_order);

      WLib::serialize(sink, ref_value_float, byte_order);
      WLib::serialize(sink, ref_value_double, byte_order);
      WLib::serialize(sink, ref_value_long_double, byte_order);
    }
    {
      WLib::byte_buffer_source_t source{ buffer };

      REQUIRE(source.get_number_of_remaining_bytes() == 1024);

      std::byte          val_byte         = WLib::deserialize<std::byte>(source, byte_order);
      char               val_char         = WLib::deserialize<char>(source, byte_order);
      char16_t           val_char16_t     = WLib::deserialize<char16_t>(source, byte_order);
      char32_t           val_char32_t     = WLib::deserialize<char32_t>(source, byte_order);

      signed char        val_s_char       = WLib::deserialize<signed char>(source, byte_order);
      signed short       val_s_short      = WLib::deserialize<signed short>(source, byte_order);
      signed int         val_s_int        = WLib::deserialize<signed int>(source, byte_order);
      signed long        val_s_long       = WLib::deserialize<signed long>(source, byte_order);
      signed long long   val_s_long_long  = WLib::deserialize<signed long long>(source, byte_order);
      
      unsigned char      val_us_char      = WLib::deserialize<unsigned char>(source, byte_order);
      unsigned short     val_us_short     = WLib::deserialize<unsigned short>(source, byte_order);
      unsigned int       val_us_int       = WLib::deserialize<unsigned int>(source, byte_order);
      unsigned long      val_us_long      = WLib::deserialize<unsigned long>(source, byte_order);
      unsigned long long val_us_long_long = WLib::deserialize<unsigned long long>(source, byte_order);
      
      float              val_float        = WLib::deserialize<float>(source, byte_order);
      double             val_double       = WLib::deserialize<double>(source, byte_order);
      long double        val_long_double  = WLib::deserialize<long double>(source, byte_order);

      REQUIRE(val_byte == ref_value_byte);
      REQUIRE(val_char == ref_value_char);
      REQUIRE(val_char16_t == ref_value_char16_t);
      REQUIRE(val_char32_t == ref_value_char32_t);
      
      REQUIRE(val_s_char == ref_value_s_char);
      REQUIRE(val_s_short == ref_value_s_short);
      REQUIRE(val_s_int == ref_value_s_int);
      REQUIRE(val_s_long == ref_value_s_long);
      REQUIRE(val_s_long_long == ref_value_s_long_long);
      
      REQUIRE(val_us_char == ref_value_us_char);
      REQUIRE(val_us_short == ref_value_us_short);
      REQUIRE(val_us_int == ref_value_us_int);
      REQUIRE(val_us_long == ref_value_us_long);
      REQUIRE(val_us_long_long == ref_value_us_long_long);
      
      REQUIRE(val_float == ref_value_float);
      REQUIRE(val_double == ref_value_double);
      REQUIRE(val_long_double == ref_value_long_double);
    }
  }
}
