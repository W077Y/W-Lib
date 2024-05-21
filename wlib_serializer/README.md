
# WLib Serializer

The WLib Serializer is a header-only C++ library designed for serialization and deserialization of a wide range of data types, emphasizing ease of use, efficiency, and flexibility.

## Prerequisites

- C++20 or higher

## Usage Examples

### Basic Serialization

```cpp
#include <wlib_serializer.hpp>

int main() {
    std::byte buffer[1024];
    wlib::ByteSink_ExBuffer sink(buffer);

    int value = 12345;
    wlib::serialize(sink, value);
}
```

### Basic Deserialization

```cpp
#include <wlib_serializer.hpp>

int main() {
    std::byte buffer[1024]; // Assume buffer is filled with serialized data.
    wlib::ByteSource_ExBuffer source(buffer);

    auto result = wlib::deserialize<int>(source);
    if (result) {
        std::cout << "Deserialized value: " << *result << std::endl;
    }
}
```

### Extending Serialization for Custom Structures

You can extend the serialization capabilities to handle custom data structures:

```cpp
#include <wlib_serializer.hpp>

struct tst_type {
    uint8_t  a;
    uint32_t b;
    uint64_t c;
};

namespace wlib {
    std::size_t serialize(ByteSink_Interface& sink, const tst_type& value, std::endian endian = std::endian::little) {
        std::size_t len = 0;
        len += serialize(sink, value.a, endian);
        len += serialize(sink, value.b, endian);
        len += serialize(sink, value.c, endian);
        return len;
    }

    template <>
    std::optional<tst_type> deserialize<tst_type>(ByteSource_Interface& source, std::endian const& endian) {
        auto a = deserialize<uint8_t>(source, endian);
        auto b = deserialize<uint32_t>(source, endian);
        auto c = deserialize<uint64_t>(source, endian);
        if (a.has_value() && b.has_value() && c.has_value()) {
            return tst_type{ a.value(), b.value(), c.value() };
        }
        return std::nullopt;
    }
}
```
