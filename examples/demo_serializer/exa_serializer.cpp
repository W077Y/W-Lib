#include <WLib_serializer.hpp>
#include <iostream>

int main()
{
  WLib::null_sink_t   sink_a;
  int                 no_sink_a;
  WLib::null_source_t source_a;
  float               no_source_a;

  WLib::serialize(sink_a, 0xdd);
  WLib::serialize(sink_a, static_cast<int>(0xAABBCCDD));

  char c;
  c = WLib::deserialize<WLib::null_source_t, char>(source_a);
  c = WLib::deserialize<char>(source_a);
  c = WLib::deserialize<int>(source_a);



  return 0;
}
