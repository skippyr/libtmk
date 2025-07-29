#include "tmtk.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#define REDIRECTION_CACHE(fd) (!_isatty(fd) << fd)
#else
#define REDIRECTION_CACHE(fd) (!isatty(fd) << fd)
#endif
#define HAS_INIT (1 << 7)
#define HAS_ANSI_CACHE (1 << 8)
#define IN 0
#define OUT 1
#define ERR 2

static void init(void);
#ifdef _WIN32
static int enable_ansi_parse(DWORD handle_id);
#endif
static void f_write_args(FILE *f, const char *fmt, va_list args);

static char cache = 0;

static void init(void) {
  if (cache & HAS_INIT) {
    return;
  }
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  !enable_ansi_parse(STD_OUTPUT_HANDLE) && enable_ansi_parse(STD_ERROR_HANDLE);
#endif
  cache = HAS_INIT | REDIRECTION_CACHE(IN) | REDIRECTION_CACHE(OUT) | REDIRECTION_CACHE(ERR);
}

#ifdef _WIN32
static int enable_ansi_parse(DWORD handle_id) {
  HANDLE handle = GetStdHandle(handle_id);
  DWORD mode;
  return GetConsoleMode(handle, &mode) && SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#endif

int is_in_redirected(void) {
  init();
  return !!(cache & IN);
}

int is_out_redirected(void) {
  init();
  return !!(cache & OUT);
}

int is_error_redirected(void) {
  init();
  return !!(cache & ERR);
}

static void f_write_args(FILE *f, const char *fmt, va_list args) {
  va_list args_cp;
  va_copy(args_cp, args);
  int len = vsnprintf(NULL, 0, fmt, args_cp);
  va_end(args_cp);
  char *buf = malloc(len + 1);
  if (!buf) {
    return;
  }
  vsnprintf(buf, len + 1, fmt, args);
  if (buf[len] == '\n') {
    cache |= HAS_ANSI_CACHE;
  }
  fputs(buf, f);
  free(buf);
}

void out_write_args(const char *fmt, va_list args) {
  f_write_args(stdout, fmt, args);
}

void out_write(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  f_write_args(stdout, fmt, args);
  va_end(args);
}

void err_write_args(const char *fmt, va_list args) {
  f_write_args(stderr, fmt, args);
}

void err_write(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  f_write_args(stderr, fmt, args);
  va_end(args);
}
