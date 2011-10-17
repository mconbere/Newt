// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#pragma once

#if defined(__APPLE__)
#include <libkern/OSAtomic.h>
#elif defined(_WIN32)
#include <Windows.h>
#else
#error Not defined for your platform
#endif

namespace newt {

class Refcount {
 public:
  Refcount() : count_(1) {}
  virtual ~Refcount() {}

  inline void Retain() {
#ifdef __APPLE__
    OSAtomicIncrement32(&count_);
#elif defined(_WIN32)
    InterlockedIncrement(&count_);
#endif
  }
  
  inline void Release() {
#if defined(__APPLE__)
    if (OSAtomicDecrement32(&count_) == 0)
#elif defined(_WIN32)
    if (InterlockedDecrement(&count_) == 0)
#endif
      delete this;
  }

 private:
#if defined(__APPLE__)
  volatile int32_t count_;
#elif defined(_WIN32)
  volatile LONG count_;
#endif
};
  
}  // namespace newt