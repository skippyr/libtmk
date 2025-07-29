#include "tmtk.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#define TTYCACHE(fd) (!!_isatty(fd) << fd)
#else
#define TTYCACHE(fd) (isatty(fd) << fd)
#endif
#define HASINIT (1 << 7)
#define HASCACHE (1 << 6)
#define PREFERSOUT (1 << 5)
#define IN 0
#define OUT 1
#define ERR 2

static void init(void);
#ifdef _WIN32
static int enbansi(DWORD handle_id);
#endif
static void vfwrite(FILE *f, const char *fmt, va_list args);
static int ansi(const char *fmt, ...);

static char cache = 0;

static void init(void) {
  if (cache & HASINIT) {
    return;
  }
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  !enbansi(STD_OUTPUT_HANDLE) && enbansi(STD_ERROR_HANDLE);
#endif
  cache = HASINIT | TTYCACHE(IN) | TTYCACHE(OUT) | TTYCACHE(ERR);
}

#ifdef _WIN32
static int enbansi(DWORD hid) {
  HANDLE h = GetStdHandle(hid);
  DWORD m;
  return GetConsoleMode(h, &m) && SetConsoleMode(h, m | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#endif

static void vfwrite(FILE *f, const char *fmt, va_list args) {
  char sbuf[1024];
  char *buf;
  va_list argscp;
  va_copy(argscp, args);
  int len = vsnprintf(NULL, 0, fmt, argscp);
  va_end(argscp);
  if (len < 1024) {
    buf = sbuf;
  } else {
    char *mbuf = malloc(len + 1);
    if (!mbuf) {
      return;
    }
    buf = mbuf;
  }
  vsnprintf(buf, len + 1, fmt, args);
  if (f == stdout) {
    if (buf[len] == '\n') {
      cache &= ~HASCACHE;
    }
    cache |= PREFERSOUT;
  } else if (f == stderr) {
    if (cache & HASCACHE) {
      flush();
    }
    cache &= ~PREFERSOUT;
  }
  fputs(buf, f);
  if (buf != sbuf) {
    free(buf);
  }
}

static int ansi(const char *fmt, ...) {
  va_list args;
  if (cache & PREFERSOUT) {
    if (outtty()) {
      va_start(args, fmt);
      vwrt(fmt, args);
      va_end(args);
      cache |= HASCACHE;
    } else if (errtty()) {
      va_start(args, fmt);
      vewrt(fmt, args);
      va_end(args);
    } else {
      return -1;
    }
  } else {
    if (errtty()) {
      va_start(args, fmt);
      vewrt(fmt, args);
      va_end(args);
    } else if (outtty()) {
      va_start(args, fmt);
      vwrt(fmt, args);
      va_end(args);
      cache |= HASCACHE;
    } else {
      return -1;
    }
  }
  return 0;
}

int intty(void) {
  init();
  return cache & IN;
}

int outtty(void) {
  init();
  return !!(cache & OUT);
}

int errtty(void) {
  init();
  return !!(cache & ERR);
}

void flush(void) {
  fflush(stdout);
  cache &= ~HASCACHE;
}

void vwrt(const char *fmt, va_list args) {
  vfwrite(stdout, fmt, args);
}

void wrt(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vwrt(fmt, args);
  va_end(args);
}

void vewrt(const char *fmt, va_list args) {
  vfwrite(stderr, fmt, args);
}

void ewrt(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vewrt(fmt, args);
  va_end(args);
}

void ansifg(int clr) {
  ansi("\x1b[38;5;%dm", clr);
}

void ansibg(int clr) {
  ansi("\x1b[48;5;%dm", clr);
}

void rgbfg(unsigned char r, unsigned char g, unsigned char b) {
  ansi("\x1b[38;2;%d;%d;%dm", r, g, b);
}

void rgbbg(unsigned char r, unsigned char g, unsigned char b) {
  ansi("\x1b[48;2;%d;%d;%dm", r, g, b);
}

void eff(int e) {
  for (int i = 0; i < 8; ++i) {
    if (e & 1 << i) {
      switch (i) {
        case 0:
          ansi("\x1b[22;1m");
          break;
        case 1:
          ansi("\x1b[22;2m");
          break;
        case 2:
          ansi("\x1b[3m");
          break;
        case 3:
          ansi("\x1b[4m");
          break;
        case 4:
          ansi("\x1b[9m");
          break;
        case 5:
          ansi("\x1b[5m");
          break;
        case 6:
          ansi("\x1b[7m");
          break;
        case 7:
          ansi("\x1b[8m");
      }
    }
  }
}

void res(void) {
  ansi("\x1b[0m");
}

void bell(void) {
  ansi("\7");
}

void cls(void) {
  ansi("\x1b[H\x1b[2J");
}

void clsln(void) {
  ansi("\x1b[G\x1b[2K");
}

void clsbuf(void) {
  ansi("\x1b[H\x1b[3J");
}
