#include "exa_serializer.hpp"
#include <WLib_serializer.hpp>
#include <WLib_Byte_Buffer_Sink.hpp>
#include <iostream>

int main()
{
  int ret = 0;
  ret |= example_serializer_min();
  ret |= example_deserializer_min();
  ret |= example_de_serializer_custom_type();
  return ret;
}
