#include "tmtk.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#define REDIRECTION_CACHE(stream_a) (!_isatty(stream_a) << stream_a)
#else
#define REDIRECTION_CACHE(stream_a) (!isatty(stream_a) << stream_a)
#endif
#define HAS_INITIATED_FLAG (1 << 7)
#define HAS_ANSI_CACHE_FLAG (1 << 6)
#define ANSI_PREFERS_STDOUT_FLAG (1 << 5)

static void init(void);
#ifdef _WIN32
static void enableANSIParse(DWORD handleID);
#endif

static char cache_g = 0;

static void init(void) {
  if (cache_g & HAS_INITIATED_FLAG) {
    return;
  }
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  !enableAnsiParse(STD_OUTPUT_HANDLE) && enableAnsiParse(STD_ERROR_HANDLE);
#endif
  cache_g = HAS_INITIATED_FLAG | REDIRECTION_CACHE(Stream_Input) | REDIRECTION_CACHE(Stream_Output) | REDIRECTION_CACHE(Stream_Error);
}

#ifdef _WIN32
static void enableANSIParse(DWORD handleID) {
  HANDLE handle = GetStdHandle(handleID);
  DWORD mode;
  return GetConsoleMode(handle, &mode) && SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#endif

int isStreamRedirected(int stream) {
  init();
  return !!(cache_g & 1 << stream);
}
