#include "tmk.h"
#include <stdio.h>
#if defined(_WIN32)
#include <Windows.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#endif

#define HAS_CACHED_ANSI_FLAG (1 << 4)
#define HAS_INITIALIZED_FLAG (1 << 7)
#if defined(_WIN32)
#define REDIRECTION_CACHE(stream_p) (!_isatty(stream_p) << stream_p)
#else
#define RAW_MODE_C_LFLAGS (ICANON | ECHO | ISIG)
#define RAW_MODE_C_IFLAGS IXON
#define REDIRECTION_CACHE(stream_p) (!isatty(stream_p) << stream_p)
#endif

#if defined(_WIN32)
static void enableAnsiParse(void);
#else
static void enableRawMode(void);
static void disableRawMode(void);
static void enableBlockingInput(void);
static void disableBlockingInput(void);
#endif
static void cacheStreamRedirections(void);
static void initialize(void);

static char cache_g = 0;

#if defined(_WIN32)
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

void tmk_flushOutputBuffer(void) {
  fflush(stdout);
}

void tmk_clearInputBuffer(void) {
#if defined(_WIN32)
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

#if defined(_WIN32)
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
