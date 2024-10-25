#include "tmk.h"
#if defined(_WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#endif

#if defined(_WIN32)
#define ISATTY(fileno_a) _isatty(fileno_a)
#else
#define ISATTY(fileno_a) isatty(fileno_a)
#endif

namespace tmk {
  bool Terminal::isInputRedirected_m = false;
  bool Terminal::isOutputRedirected_m = false;
  bool Terminal::isErrorRedirected_m = false;
  bool Terminal::hasSetup_m = false;

#if defined(_WIN32)
  void Terminal::setUtf8Encoding() {
    SetConsoleOutputCP(CP_UTF8);
  }

  void Terminal::enableAnsiParse() {
    HANDLE handle;
    DWORD mode;
    (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
     GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
  }
#endif

  void Terminal::setup() {
    if (hasSetup_m) {
      return;
    }
    hasSetup_m = true;
#if defined(_WIN32)
    setUtf8Encoding();
    enableAnsiParse();
#endif
    isInputRedirected_m = !ISATTY(0);
    isOutputRedirected_m = !ISATTY(1);
    isErrorRedirected_m = !ISATTY(2);
  }

  bool Terminal::isInputRedirected() {
    setup();
    return isInputRedirected_m;
  }

  bool Terminal::isOutputRedirected() {
    setup();
    return isOutputRedirected_m;
  }

  bool Terminal::isErrorRedirected() {
    setup();
    return isErrorRedirected_m;
  }
}
