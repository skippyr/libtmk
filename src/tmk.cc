#include "tmk.hh"
#if defined(_WIN32)
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#if defined(_WIN32)
#define IS_STREAM_REDIRECTED(stream_a) (!_isatty(stream_a) << stream_a)
#else
#define IS_STREAM_REDIRECTED(stream_a) (!isatty(stream_a) << stream_a)
#endif

namespace tmk {
uint8_t Terminal::cache_m = 0;

#if defined(_WIN32)
void Terminal::enableAnsiParse() noexcept {
  HANDLE handle;
  DWORD mode;
  (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
   GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
      SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#endif

void Terminal::cacheStreamStates() noexcept {
  cache_m |= IS_STREAM_REDIRECTED(0) | IS_STREAM_REDIRECTED(1) |
             IS_STREAM_REDIRECTED(2);
}

void Terminal::init() noexcept {
  if (cache_m & 1 << 7) {
    return;
  }
  cache_m |= 1 << 7;
#if defined(_WIN32)
  SetConsoleOutputCP(CP_UTF8);
  enableAnsiParse();
#endif
  cacheStreamStates();
}

void Terminal::flushOutput() noexcept { std::fflush(stdout); }

bool Terminal::isInputRedirected() noexcept {
  init();
  return cache_m & 1;
}

bool Terminal::isOutputRedirected() noexcept {
  init();
  return cache_m & 1 << 1;
}

bool Terminal::isErrorRedirected() noexcept {
  init();
  return cache_m & 1 << 2;
}

void Terminal::setFontColor(AnsiColor color, Layer layer) noexcept {
  try {
    writeAnsi("\x1b[{}8;5;{}m", static_cast<int>(layer),
              static_cast<int>(color));
  } catch (...) {
  }
}

void Terminal::resetFontColors() noexcept {
  try {
    writeAnsi("\x1b[39;49m");
  } catch (...) {
  }
}

void Terminal::writeLine() noexcept {
  init();
  cache_m &= ~(1 << 4);
  try {
    std::cout << std::endl;
  } catch (...) {
  }
}

void Terminal::writeErrorLine() noexcept {
  init();
  if (cache_m & 1 << 4) {
    cache_m &= ~(1 << 4);
    flushOutput();
  }
  try {
    std::cerr << std::endl;
  } catch (...) {
  }
}
} // namespace tmk
