#pragma once
#ifndef WLIB_CSTRING_SINK_INTERFACE_HPP
#define WLIB_CSTRING_SINK_INTERFACE_HPP

namespace wlib
{
  class CString_Sink_Interface
  {
  public:
    virtual ~CString_Sink_Interface() = default;

    virtual bool operator()(char const* c_str) = 0;
  };
}    // namespace wlib

#endif
