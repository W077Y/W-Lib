# WLib Serializer/Deserializer

Die WLib Serializer/Deserializer-Bibliothek bietet Funktionen zum Serialisieren und Deserialisieren von Daten.
Die Konzepte `byte_sink_t` und `byte_source_t` bilden hierfür die Grundlage.
Um zu überprüfen, ob der Typ `T`das Konzept erfüllt, kann `is_byte_sink_v<T>` bzw. `is_byte_source_v<T>` verwendet werden.

Beispiel:
```cpp
  static_assert(is_byte_sink_v<byte_buffer_sink_t>);
  static_assert(is_byte_source_v<byte_buffer_source_t>);
```

## Serialisieren

Verwenden Sie die Funktion `WLib::serialize`, um Daten zu serialisieren. Sie akzeptiert einen `byte_sink_t`, den zu serialisierenden Wert und optional die Byte-Reihenfolge.
Beispiel:

```cpp
#include <WLib_serializer.hpp>
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

    WLib::serialize(sink, value, WLib::ByteOrder::little_endian);

    assert(sink.get_number_of_used_bytes() == 8);
    assert(sink.get_number_of_remaining_bytes() == 120);
  }

  assert(tmp[0] == static_cast<std::byte>(0x00));
  assert(tmp[1] == static_cast<std::byte>(0x00));
  assert(tmp[2] == static_cast<std::byte>(0x00));
  assert(tmp[3] == static_cast<std::byte>(0x42));
  assert(tmp[4] == static_cast<std::byte>(0x42));
  assert(tmp[5] == static_cast<std::byte>(0x00));
  assert(tmp[6] == static_cast<std::byte>(0x00));
  assert(tmp[7] == static_cast<std::byte>(0x00));

  return 0;
}
```

## Deserialisieren

Mit der Funktion `WLib::deserialize<T>` können Daten deserialisiert werden. Sie akzeptiert einen `byte_source_t` und optional die Byte-Reihenfolge. Die Funktion gibt den deserialisierten Wert zurück.

Beispiel:
```cpp
#include <WLib_serializer.hpp>
#include <cassert>

int example_deserializer_min()
{
  std::byte tmp[128] = {
    static_cast<std::byte>(0x00), static_cast<std::byte>(0x00), static_cast<std::byte>(0x00), static_cast<std::byte>(0x42),
    static_cast<std::byte>(0x42), static_cast<std::byte>(0x00), static_cast<std::byte>(0x00), static_cast<std::byte>(0x00),
  };

  {
    WLib::byte_buffer_source_t sink{ tmp };
    assert(sink.get_number_of_remaining_bytes() == 128);

    int32_t value_1 = WLib::deserialize<int32_t>(sink, WLib::ByteOrder::big_endian);
    assert(sink.get_number_of_processed_bytes() == 4);
    assert(sink.get_number_of_remaining_bytes() == 124);

    int32_t value_2 = WLib::deserialize<int32_t>(sink, WLib::ByteOrder::little_endian);
    assert(sink.get_number_of_processed_bytes() == 8);
    assert(sink.get_number_of_remaining_bytes() == 120);

    assert(value_1 == 0x42);
    assert(value_2 == 0x42);
  }
  return 0;
}
```


## Anpassung an custom typs

Um die Bibliothek um einen benutzerdefinierten Typen zu erweitern, muss lediglich das entsprechende `type_trait` hinzugefügt werden.

Beispiel:

Angenommen, wir haben den Typ `my_type`.

```cpp
// exa_my_type.hpp
#pragma once
#ifndef EXA_MY_TYPE_HPP
#define	EXA_MY_TYPE_HPP

#include <cstdint>

struct my_type
{
public:
  constexpr my_type(char a, uint32_t b, uint64_t c, char d)
      : m_a(a)
      , m_b(b)
      , m_c(c)
      , m_d(d)
  {
  }

  constexpr uint64_t get_value() const { return this->m_a + this->m_b + this->m_c + this->m_d; }

  char     m_a;
  uint32_t m_b;
  uint64_t m_c;
  char     m_d;
};

#endif
```

Nun fügen wir die Spezialisierungen für `serializer_traits` und `deserializer_traits` hinzu.

```cpp
// exa_serializer_custom.cpp

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
```

Jetzt können wir den Typen nach Belieben serialisieren und deserialisieren.

```cpp
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
```



