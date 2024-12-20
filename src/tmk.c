#include "tmk.h"
#if defined(_WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define HAS_CACHED_ANSI_FLAG (1 << 4)
#define HAS_INITIALIZED_FLAG (1 << 7)
#if defined(_WIN32)
#define REDIRECTION_CACHE(stream_p) (!_isatty(stream_p) << stream_p)
#else
#define REDIRECTION_CACHE(stream_p) (!isatty(stream_p) << stream_p)
#endif

#if defined(_WIN32)
static void enableAnsiParse(void);
#endif
static void cacheStreamRedirections(void);
static void initialize(void);

static char cache_g = 0;

static void cacheStreamRedirections(void) {
  cache_g |= REDIRECTION_CACHE(tmk_Stream_Input) |
             REDIRECTION_CACHE(tmk_Stream_Output) |
             REDIRECTION_CACHE(tmk_Stream_Error);
}

static void initialize(void) {
  if (cache_g & HAS_INITIALIZED_FLAG) {
    return;
  }
  cache_g |= HAS_INITIALIZED_FLAG;
#if defined(_WIN32)
  SetConsoleOutputCP(CP_UTF8);
  enableAnsiParse();
#endif
  cacheStreamRedirections();
}

int tmk_isStreamRedirected(int stream) {
  initialize();
  return !!(cache_g & 1 << stream);
}
