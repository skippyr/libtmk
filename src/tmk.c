#include "tmk.h"
#include <stdio.h>
#if tmk_IS_WINDOWS_OS
#include <Windows.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#endif

#define HAS_CACHED_ANSI_FLAG (1 << 4)
#define HAS_INITIALIZED_FLAG (1 << 7)
#if tmk_IS_WINDOWS_OS
#define REDIRECTION_CACHE(stream_p) (!_isatty(stream_p) << stream_p)
#else
#define RAW_MODE_C_LFLAGS (ICANON | ECHO | ISIG)
#define RAW_MODE_C_IFLAGS IXON
#define REDIRECTION_CACHE(stream_p) (!isatty(stream_p) << stream_p)
#endif

#if tmk_IS_WINDOWS_OS
static void enableAnsiParse(void);
#else
static void enableRawMode(void);
static void disableRawMode(void);
static void enableBlockingInput(void);
static void disableBlockingInput(void);
#endif
static void cacheStreamRedirections(void);
static int writeAnsi(const char *format, ...);
static void initialize(void);

static char cache_g = 0;

#if tmk_IS_WINDOWS_OS
static void enableAnsiParse(void) {
    HANDLE handle;
    DWORD mode;
    (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
     GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
static void enableRawMode(void) {
  struct termios attributes;
  tcgetattr(STDIN_FILENO, &attributes);
  attributes.c_lflag &= ~RAW_MODE_C_LFLAGS;
  attributes.c_iflag &= ~RAW_MODE_C_IFLAGS;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
}

static void disableRawMode(void) {
  struct termios attributes;
  tcgetattr(STDIN_FILENO, &attributes);
  attributes.c_lflag |= RAW_MODE_C_LFLAGS;
  attributes.c_iflag |= RAW_MODE_C_IFLAGS;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
}

static void enableBlockingInput(void) {
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}

static void disableBlockingInput(void) {
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}
#endif

static void cacheStreamRedirections(void) {
  cache_g |= REDIRECTION_CACHE(tmk_Stream_Input) |
             REDIRECTION_CACHE(tmk_Stream_Output) |
             REDIRECTION_CACHE(tmk_Stream_Error);
}

static int writeAnsi(const char *format, ...) {
  if (!tmk_isStreamRedirected(tmk_Stream_Output)) {
    va_list arguments;
    va_start(arguments, format);
    tmk_writeArguments(format, arguments);
    va_end(arguments);
    cache_g |= HAS_CACHED_ANSI_FLAG;
  } else if (!tmk_isStreamRedirected(tmk_Stream_Error)) {
    va_list arguments;
    va_start(arguments, format);
    tmk_writeErrorArguments(format, arguments);
    va_end(arguments);
  } else {
    return -1;
  }
  return 0;
}

static void initialize(void) {
  if (cache_g & HAS_INITIALIZED_FLAG) {
    return;
  }
  cache_g |= HAS_INITIALIZED_FLAG;
#if tmk_IS_WINDOWS_OS
  SetConsoleOutputCP(CP_UTF8);
  enableAnsiParse();
#endif
  cacheStreamRedirections();
}

int tmk_isStreamRedirected(int stream) {
  initialize();
  return !!(cache_g & 1 << stream);
}

void tmk_flushOutputBuffer(void) {
  fflush(stdout);
  cache_g &= ~HAS_CACHED_ANSI_FLAG;
}

void tmk_clearInputBuffer(void) {
#if tmk_IS_WINDOWS_OS
  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
  enableRawMode();
  disableBlockingInput();
  while (getchar() != EOF) {
  }
  enableBlockingInput();
  disableRawMode();
#endif
}

#if tmk_IS_WINDOWS_OS
char *tmk_convertUtf16ToUtf8(const wchar_t *utf16String, size_t *length) {
  int size = WideCharToMultiByte(CP_UTF8, 0, utf16String, -1, NULL, 0, NULL,
                                 NULL);
  char *buffer = malloc(size);
  WideCharToMultiByte(CP_UTF8, 0, utf16String, -1, buffer, size, NULL, NULL);
  if (length) {
    *length = size - 1;
  }
  return buffer;
}

wchar_t *tmk_convertUtf8ToUtf16(const char *utf8String, size_t *length) {
  int size = MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, NULL, 0);
  wchar_t *buffer = malloc(size * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, buffer, size);
  if (length) {
    *length = size - 1;
  }
  return buffer;
}
#endif

void tmk_getArguments(int argc, const char **argv,
                      struct tmk_Arguments *arguments) {
#if defined(_WIN32)
  arguments->asUtf16 = CommandLineToArgvW(GetCommandLineW(), &arguments->total);
  arguments->asUtf8 = malloc(arguments->total * sizeof(void *));
  for (int offset = 0; offset < arguments->total; ++offset) {
    arguments->asUtf8[offset] =
        tmk_convertUtf16ToUtf8(arguments->asUtf16[offset]);
  }
#else
  arguments->total = argc;
  arguments->asUtf8 = argv;
#endif
}

void tmk_freeArguments(struct tmk_Arguments *arguments) {
#if defined(_WIN32)
  LocalFree(arguments->asUtf16);
  for (int offset = 0; offset < arguments->total; ++offset) {
    free((void *)arguments->asUtf8[offset]);
  }
  free(arguments->asUtf8);
#endif
}

void tmk_writeArguments(const char *format, va_list arguments) {
  initialize();
  vprintf(format, arguments);
}

void tmk_writeArgumentsLine(const char *format, va_list arguments) {
  tmk_writeArguments(format, arguments);
  tmk_write("\n");
  cache_g &= ~HAS_CACHED_ANSI_FLAG;
}

void tmk_write(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_writeArguments(format, arguments);
  va_end(arguments);
}

void tmk_writeLine(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_writeArgumentsLine(format, arguments);
  va_end(arguments);
}

void tmk_writeErrorArguments(const char *format, va_list arguments) {
  initialize();
  if (cache_g & HAS_CACHED_ANSI_FLAG) {
    tmk_flushOutputBuffer();
  }
  vfprintf(stderr, format, arguments);
}

void tmk_writeErrorArgumentsLine(const char *format, va_list arguments) {
  tmk_writeErrorArguments(format, arguments);
  tmk_writeError("\n");
}

void tmk_writeError(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_writeErrorArguments(format, arguments);
  va_end(arguments);
}

void tmk_writeErrorLine(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_writeErrorArgumentsLine(format, arguments);
  va_end(arguments);
}
