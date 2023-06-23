# WLib Serializer/Deserializer

Die WLib Serializer/Deserializer-Bibliothek bietet Funktionen zum Serialisieren und Deserialisieren von Daten. Die Konzepte
`byte_sink_t` und `byte_source_t` stellen hierfür die Basis dar.

Eine `byte_sink_t` bietet über den call operator die Möglichkeit ein Byte aufzunehmen.

```cpp
struct byte_sink_t
{
 void operator()(std::byte const&);
};
```

Eine `byte_source_t` bietet über den call operator die Möglichkeit ein Byte anzufragen.
```cpp
struct byte_source_t
{
 std::byte operator()();
};
```

Über die Implementierung der Konzepte kann die `byte_sink` bzw. `byte_source` den genauen umständen angepasst werden.

## `byte_buffer_sink_t`

## `byte_buffer_source_t`

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
```



