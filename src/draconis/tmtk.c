#include "tmtk.h"
#include <stdio.h>
#include <stdlib.h>
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

#ifdef _WIN32
static void enableANSIParse(DWORD handleID);
#endif
static void init(void);
static void streamWriteArguments(FILE *stream, const char *format, va_list arguments);
static void streamWrite(FILE *stream, const char *format, ...);
static void ansiWrite(const char *format, ...);

static char cache = 0;

#ifdef _WIN32
static void enableANSIParse(DWORD handleID) {
  HANDLE handle = GetStdHandle(handleID);
  DWORD mode;
  return GetConsoleMode(handle, &mode) && SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#endif

static void init(void) {
  if (cache & HAS_INITIATED_FLAG) {
    return;
  }
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  !enableAnsiParse(STD_OUTPUT_HANDLE) && enableAnsiParse(STD_ERROR_HANDLE);
#endif
  cache = HAS_INITIATED_FLAG | REDIRECTION_CACHE(Stream_Input) | REDIRECTION_CACHE(Stream_Output) | REDIRECTION_CACHE(Stream_Error);
}

static void streamWriteArguments(FILE *stream, const char *format, va_list arguments) {
  char staticBuffer[1024];
  char *buffer;
  va_list argumentsCopy;
  va_copy(argumentsCopy, arguments);
  int size = vsnprintf(NULL, 0, format, argumentsCopy) + 1;
  va_end(argumentsCopy);
  if (size < 1024) {
    buffer = staticBuffer;
  } else if (!(buffer = malloc(size))) {
    return;
  }
  vsnprintf(buffer, size, format, arguments);
  if (stream == stdout) {
    if (buffer[size - 1] == '\n') {
      cache &= ~HAS_ANSI_CACHE_FLAG;
    }
    cache |= ANSI_PREFERS_STDOUT_FLAG;
  } else {
    if (cache & HAS_ANSI_CACHE_FLAG) {
      flushOutput();
    }
    cache &= ~ANSI_PREFERS_STDOUT_FLAG;
  }
  fputs(buffer, stream);
  if (buffer != staticBuffer) {
    free(buffer);
  }
}

static void streamWrite(FILE *stream, const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  streamWriteArguments(stream, format, arguments);
  va_end(arguments);
}

int isStreamRedirected(int stream) {
  init();
  return !!(cache & 1 << stream);
}

void flushOutput(void) {
  fflush(stdout);
  cache &= ~HAS_ANSI_CACHE_FLAG;
}

void outputWriteArguments(const char *format, va_list arguments) {
  streamWriteArguments(stdout, format, arguments);
}

void outputWrite(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  outputWriteArguments(format, arguments);
  va_end(arguments);
}

void errorWriteArguments(const char *format, va_list arguments) {
  streamWriteArguments(stderr, format, arguments);
}

void errorWrite(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  errorWriteArguments(format, arguments);
  va_end(arguments);
}
