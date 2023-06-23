# WLib Serializer/Deserializer

WLib Serializer/Deserializer bietet Funktionen zum Serialisieren und Deserialisieren von Daten in/aus einem binären Daten-BLOB.

## Serialisieren

Verwenden Sie die Funktion `WLib::serialize`, um Daten zu serialisieren.

Beispiel:
```cpp
#include <WLib_serializer.hpp>
#include <cassert>

int example_serializer_min()
{
  std::byte tmp[128] = {};

  {
    WLib::byte_sink_t sink{ tmp };
    int32_t           value = 0x42;
    WLib::serialize(sink, value, WLib::ByteOrder::big_endian);
    
    assert(sink.number_of_used_bytes() == 4);
    assert(sink.number_of_free_bytes() == 124);
  }

  assert(tmp[0] == static_cast<std::byte>(0x00));
  assert(tmp[1] == static_cast<std::byte>(0x00));
  assert(tmp[2] == static_cast<std::byte>(0x00));
  assert(tmp[3] == static_cast<std::byte>(0x42));

  return 0;
}
```
## Deserialisieren

Mit der Funktion `WLib::deserialize<T>` können Daten deserialisiert werden.

Beispiel:



