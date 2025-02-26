#pragma once

#include <iostream>
#include <sstream>

static constexpr uint32_t ONE_MEGA_BYTE = 1 << 20;
static constexpr uint32_t ONE_GIGA_BYTE = 1 << 30;

struct ExitMessage {
  __attribute__((noreturn, noinline)) ~ExitMessage() {
    std::cerr << ss_.str() << std::endl;
    exit(1);
  }

  template<class T>
  ExitMessage& operator<<(const T& t) {
    ss_ << t;
    return *this;
  }
  operator bool() const {
    return true;
  }

 private:
  std::ostringstream ss_;
};

#define ASSERT(cond)                              \
  __builtin_expect(static_cast<bool>(cond), 1) || \
      ExitMessage() << "==========\nASSERT FAILURE " << __FILE__ << ":" << __LINE__ << " "

#define INLINE __attribute__((always_inline)) inline  // gcc - TODO(lboehm): add other compilers
