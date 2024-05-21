#pragma once
#ifndef WLIB_OS_HPP
#define WLIB_OS_HPP

#include <semaphore>
#include <thread>

namespace os
{
  namespace this_thread
  {
    using std::this_thread::yield;
  }
  using std::binary_semaphore;
  using std::counting_semaphore;
}    // namespace os
#endif
