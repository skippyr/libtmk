#pragma once

#include <iostream>

namespace tmk {
class StreamRedirectionException : public std::exception {};

class Terminal {
private:
  static uint8_t cache_m;

#if defined(_WIN32)
  static void enableAnsiParse() noexcept;
#endif
  static void cacheStreamStates() noexcept;

public:
  static void init() noexcept;
  static bool isInputRedirected() noexcept;
  static bool isOutputRedirected() noexcept;
  static bool isErrorRedirected() noexcept;
};
}
