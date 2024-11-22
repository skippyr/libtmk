#include "tmk.h"
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#ifdef __linux__
#include <time.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#define ISATTY(std_a) !!_isatty(std_a)
#define KEY(val_a, key_a) \
  case val_a: \
    key->buf = key_a; \
    goto end_l;
#else
#ifdef __APPLE__
#define ALTKEY(val_a, key_a) \
  case val_a: \
    key->buf = key_a; \
    key->mods |= tmk_ModAlt; \
    goto end_l;
#endif
#define ISATTY(std_a) isatty(std_a)
#define UEOF 255
#define DIFFMS(start_a, end_a) \
  (((end_a.tv_sec - start_a.tv_sec) * 1000) + \
   ((end_a.tv_nsec - start_a.tv_nsec) / 1000000))
#define MAX(ni_a, nii_a) (ni_a > nii_a ? ni_a : nii_a)
#define BYTE(i_a) ((unsigned char*)&key->buf)[i_a]
#endif

static void init(void);
#ifndef _WIN32
static void setraw(int israw);
static void setblk(int isblk);
static void filtsig(int isfilt, sigset_t *bkp);
static void catchsig(int sig);
#endif
static void writeansi(const char *fmt, ...);

static char cache_g = 0;

static void
init(void)
{
  if (cache_g & 1 << 7) {
    return;
  }
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  HANDLE h;
  DWORD m;
  (GetConsoleMode((h = GetStdHandle(STD_OUTPUT_HANDLE)), &m) ||
   GetConsoleMode((h = GetStdHandle(STD_ERROR_HANDLE)), &m)) &&
      SetConsoleMode(h, m | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
  cache_g |= ISATTY(0) | ISATTY(1) << 1 | ISATTY(2) << 2 | 1 << 7;
}

#ifndef _WIN32
static void
setraw(int israw)
{
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag = israw ? t.c_lflag & ~(ICANON | ECHO | ISIG)
                    : t.c_lflag | ICANON | ECHO | ISIG;
  t.c_iflag = israw ? t.c_iflag & ~IXON : t.c_iflag | IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

static void
setblk(int isblk)
{
  int f = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL, isblk ? f & ~O_NONBLOCK : f | O_NONBLOCK);
}

static void
filtsig(int isfilt, sigset_t *bkp)
{
  struct sigaction a;
  a.sa_flags = 0;
  sigemptyset(&a.sa_mask);
  a.sa_handler = isfilt ? catchsig : SIG_DFL;
  sigaction(SIGWINCH, &a, NULL);
  if (!isfilt) {
    pthread_sigmask(SIG_SETMASK, bkp, NULL);
    return;
  }
  sigset_t filt;
  sigfillset(&filt);
  sigdelset(&filt, SIGWINCH);
  pthread_sigmask(SIG_SETMASK, &filt, bkp);
}

static void
catchsig(int sig)
{
}
#endif

static void
writeansi(const char *fmt, ...)
{
  va_list v;
  va_start(v, fmt);
  if (tmk_istty(tmk_StdOut)) {
    tmk_vwrite(fmt, v);
  } else if (tmk_istty(tmk_StdErr)) {
    tmk_vewrite(fmt, v);
  }
  va_end(v);
}

#ifdef _WIN32
char *
tmk_asutf8(const wchar_t *wstr)
{
  int sz = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
  char *buf = malloc(sz);
  WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buf, sz, NULL, NULL);
  return buf;
}
#endif

int
tmk_istty(int std)
{
  init();
  return cache_g & 1 << std;
}

void
tmk_flushout(void)
{
  fflush(stdout);
}

void
tmk_clearin(void)
{
#ifdef _WIN32
  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
  if (!tmk_istty(tmk_StdIn)) {
    return;
  }
  setraw(1);
  setblk(0);
  while (getchar() != EOF) {
  }
  setblk(1);
  setraw(0);
#endif
}

int
tmk_getwdim(struct tmk_dim *d)
{
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO b;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &b) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &b)) {
    return -1;
  }
  d->cols = b.srWindow.Right - b.srWindow.Left + 1;
  d->rows = b.srWindow.Bottom - b.srWindow.Top + 1;
#else
  struct winsize w;
  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &w) &&
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) &&
      ioctl(STDERR_FILENO, TIOCGWINSZ, &w)) {
    return -1;
  }
  d->cols = w.ws_col;
  d->rows = w.ws_row;
#endif
  return 0;
}

void
tmk_setwalt(int isalt)
{
  writeansi(isalt ? "\x1b[?1049h\x1b[2J\x1b[1;1H" : "\x1b[?1049l");
}

void
tmk_setcvis(int isvis)
{
  writeansi("\x1b[?25%c", isvis ? 'h' : 'l');
}

void
tmk_setcshp(int shp)
{
  writeansi("\x1b[%d q", shp);
}

int
tmk_getcpos(struct tmk_pos *p)
{
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO b;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &b) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &b)) {
    return -1;
  }
  p->col = b.dwCursorPosition.X - b.srWindow.Left;
  p->row = b.dwCursorPosition.Y - b.srWindow.Top;
#else
  if (!tmk_istty(tmk_StdIn) || (!tmk_istty(tmk_StdOut) &&
                                !tmk_istty(tmk_StdErr))) {
    return -1;
  }
  tmk_clearin();
  setraw(1);
  writeansi("\x1b[6n");
  unsigned short col;
  unsigned short row;
  scanf("\x1b[%hu;%huR", &row, &col);
  setraw(0);
  p->col = col - 1;
  p->row = row - 1;
#endif
  return 0;
}

void
tmk_setcpos(struct tmk_pos p)
{
  writeansi("\x1b[%hu;%huH", p.row + 1, p.col + 1);
}

void
tmk_mvcpos(unsigned short stp, int drt)
{
  writeansi("\x1b[%hu%c", stp, drt);
}

void
tmk_setclr(int clr, int lyr)
{
  writeansi(clr == tmk_ClrDft ? "\x1b[%d9m" : "\x1b[%d8;5;%dm", lyr, clr);
}

void
tmk_swpclr(int isswp)
{
  writeansi("\x1b[%dm", isswp ? 7 : 27);
}

void
tmk_setwgt(int wgt)
{
  writeansi(wgt == tmk_WgtDft ? "\x1b[22m" : "\x1b[22;%dm", wgt);
}

void
tmk_clearln(void)
{
  writeansi("\x1b[2K\x1b[1G");
}

void
tmk_ringbell(void)
{
  writeansi("\7");
}

int
tmk_readkey(int wait, int (*filt)(struct tmk_key*), struct tmk_key *key)
{
  if (!tmk_istty(tmk_StdIn) || (!tmk_istty(tmk_StdOut) &&
                                !tmk_istty(tmk_StdErr))) {
    return -1;
  }
  tmk_flushout();
  int ret = 0;
#ifdef _WIN32
  HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
  HANDLE timer = NULL;
  DWORD mode;
  GetConsoleMode(in, &mode);
  SetConsoleMode(in, mode & ~ENABLE_PROCESSED_INPUT);
  while (1) {
    if (!wait) {
      DWORD t;
      GetNumberOfConsoleInputEvents(in, &t);
      if (!t) {
        ret = -2;
        goto end_l;
      }
    } else if (wait > 0) {
      if (!timer) {
        timer = CreateWaitableTimerW(NULL, TRUE, NULL);
        LARGE_INTEGER l;
        l.QuadPart = wait * -10000;
        SetWaitableTimer(timer, &l, 0, NULL, NULL, FALSE);
      }
      HANDLE h[] = {timer, in};
      if (WaitForMultipleObjects(2, h, FALSE, INFINITE) == WAIT_OBJECT_0) {
        ret = -3;
        goto end_l;
      }
    }
    INPUT_RECORD rec;
    DWORD t;
    ReadConsoleInputW(in, &rec, 1, &t);
    if (rec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
      ret = -4;
      goto end_l;
    }
    if (rec.EventType != KEY_EVENT || !rec.Event.KeyEvent.bKeyDown ||
        rec.Event.KeyEvent.wVirtualKeyCode == VK_CONTROL ||
        rec.Event.KeyEvent.wVirtualKeyCode == VK_SHIFT ||
        rec.Event.KeyEvent.wVirtualKeyCode == VK_MENU ||
        rec.Event.KeyEvent.wVirtualKeyCode == VK_CAPITAL ||
        rec.Event.KeyEvent.wVirtualKeyCode == VK_NUMLOCK ||
        rec.Event.KeyEvent.wVirtualKeyCode == VK_SCROLL) {
      continue;
    }
    memset(key, 0, sizeof(struct tmk_key));
    ((wchar_t*)&key->buf)[0] = rec.Event.KeyEvent.uChar.UnicodeChar;
    if (rec.Event.KeyEvent.uChar.UnicodeChar) {
      if (key->buf <= 26 && key->buf != tmk_KeyTab && key->buf != tmk_KeyRet) {
        key->buf += 96;
      } else if (key->buf >= HIGH_SURROGATE_START &&
                 key->buf <= HIGH_SURROGATE_END) {
        ReadConsoleInputW(in, &rec, 1, &t);
        ReadConsoleInputW(in, &rec, 1, &t);
        ((wchar_t*)&key->buf)[1] = rec.Event.KeyEvent.uChar.UnicodeChar;
        int k = 0;
        WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)&key->buf, 2, (char*)&k, 4,
                            NULL, NULL);
        key->buf = k;
      } else {
        int k = 0;
        WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)&key->buf, 1, (char*)&k, 4,
                            NULL, NULL);
        key->buf = k;
      }
      key->mods =
          !!(rec.Event.KeyEvent.dwControlKeyState &
             (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) |
          !!(rec.Event.KeyEvent.dwControlKeyState &
             (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) << 1;
      goto end_l;
    } else if (rec.Event.KeyEvent.dwControlKeyState &
               (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED | LEFT_CTRL_PRESSED |
                RIGHT_CTRL_PRESSED | SHIFT_PRESSED)) {
      continue;
    }
    switch (rec.Event.KeyEvent.wVirtualKeyCode) {
      KEY(VK_UP, tmk_KeyUpArr);
      KEY(VK_DOWN, tmk_KeyDnArr);
      KEY(VK_RIGHT, tmk_KeyRgArr);
      KEY(VK_LEFT, tmk_KeyLfArr);
    }
    continue;
  end_l:
    if (!ret && filt && !filt(key)) {
      continue;
    }
    SetConsoleMode(in, mode);
    if (timer) {
      CloseHandle(timer);
    }
    break;
  }
#else
  sigset_t sigbkp;
  int timer;
  if (wait) {
    filtsig(1, &sigbkp);
    timer = wait;
  } else {
    setblk(0);
  }
  setraw(1);
  while (1) {
    memset(key, 0, sizeof(struct tmk_key));
    if (!wait) {
      if ((BYTE(0) = getchar()) == UEOF) {
        ret = -2;
        goto end_l;
      }
    } else if (wait < 0) {
      struct pollfd pfd = {STDIN_FILENO, POLLIN, 0};
      int s = poll(&pfd, 1, -1);
      if (s < 0) {
        ret = errno == EINTR ? -4 : -1;
        goto end_l;
      }
      BYTE(0) = getchar();
      setblk(0);
    } else {
      struct pollfd pfd = {STDIN_FILENO, POLLIN, 0};
      struct timespec start;
      struct timespec end;
      clock_gettime(CLOCK_MONOTONIC, &start);
      int s = poll(&pfd, 1, timer);
      clock_gettime(CLOCK_MONOTONIC, &end);
      if (s <= 0) {
        ret = errno == EINTR ? -4 : !s ? -3 : -1;
        goto end_l;
      }
      timer = MAX(0, timer - DIFFMS(start, end));
      BYTE(0) = getchar();
      setblk(0);
    }
#ifdef __APPLE__
    switch (BYTE(0)) {
      ALTKEY(38, 'J');
      ALTKEY(47, 'q');
      ALTKEY(60, 'Z');
      ALTKEY(62, 'X');
      ALTKEY(63, 'w');
      ALTKEY(92, 'Q');
    case 194:
      switch ((BYTE(1) = getchar())) {
        ALTKEY(160, ' ');
        ALTKEY(167, 'S');
        ALTKEY(169, 'C');
        ALTKEY(174, 'r');
        ALTKEY(181, 'm');
        ALTKEY(191, 'W');
      }
    case 195:
      switch ((BYTE(1) = getchar())) {
        ALTKEY(134, 'A');
        ALTKEY(144, 'D');
        ALTKEY(152, 'O');
        ALTKEY(158, 'P');
        ALTKEY(159, 's');
        ALTKEY(166, 'a');
        ALTKEY(176, 'd');
        ALTKEY(184, 'o');
        ALTKEY(190, 'p');
      }
    case 196:
      switch ((BYTE(1) = getchar())) {
        ALTKEY(145, 'f');
        ALTKEY(166, 'H');
        ALTKEY(167, 'h');
        ALTKEY(177, 'I');
        ALTKEY(184, 'k');
      }
    case 197:
      switch ((BYTE(1) = getchar())) {
        ALTKEY(129, 'L');
        ALTKEY(130, 'l');
        ALTKEY(138, 'N');
        ALTKEY(139, 'n');
        ALTKEY(166, 'T');
        ALTKEY(167, 't');
      }
    case 198:
      switch ((BYTE(1) = getchar())) {
        ALTKEY(178, 'V');
      }
    case 202:
      switch ((BYTE(1) = getchar())) {
        ALTKEY(139, 'v');
        ALTKEY(157, 'j');
      }
    case 203:
      switch ((BYTE(1) = getchar())) {
        ALTKEY(157, 'G');
      }
    case 206:
      switch ((BYTE(1) = getchar())) {
        ALTKEY(169, 'z');
      }
    case 226:
      switch ((BYTE(1) = getchar())) {
      case 130:
        switch ((BYTE(2) = getchar())) {
          ALTKEY(162, 'c');
          ALTKEY(172, 'e');
        }
      case 132:
        switch ((BYTE(2) = getchar())) {
          ALTKEY(162, 'B');
        }
      case 134:
        switch ((BYTE(2) = getchar())) {
          ALTKEY(144, 'y');
          ALTKEY(145, 'U');
          ALTKEY(146, 'i');
          ALTKEY(147, 'u');
        }
      case 136:
        switch ((BYTE(2) = getchar())) {
          ALTKEY(134, 'g');
          ALTKEY(171, 'b');
        }
      case 137:
        switch ((BYTE(2) = getchar())) {
          ALTKEY(136, 'x');
        }
      case 151:
        switch ((BYTE(2) = getchar())) {
          ALTKEY(138, 'F');
        }
      }
    case 239:
      switch ((BYTE(1) = getchar())) {
      case 163:
        switch ((BYTE(2) = getchar())) {
          ALTKEY(191, 'K');
        }
      }
    }
#endif
    if (BYTE(0) & 1 << 7) {
      if (BYTE(0) & 1 << 6 && !BYTE(1)) {
        BYTE(1) = getchar();
      }
      if (BYTE(0) & 1 << 5 && !BYTE(2)) {
        BYTE(2) = getchar();
      }
      if (BYTE(0) & 1 << 4 && !BYTE(3)) {
        BYTE(3) = getchar();
      }
      goto end_l;
    }
    if (BYTE(0) == 27) {
      switch ((BYTE(1) = getchar())) {
      case UEOF:
        key->buf = tmk_KeyEsc;
        goto end_l;
      case 91:
        if ((BYTE(2) = getchar()) >= 65 && BYTE(2) <= 68) {
          key->buf = BYTE(2) - 70;
          goto end_l;
        }
      case 79:
        while (getchar() != EOF) {
        }
        if (wait) {
          setblk(1);
        }
        continue;
      }
    }
    if ((key->buf = (key->mods = BYTE(0) == 27) ? BYTE(1) : BYTE(0)) >= 0 &&
        key->buf <= 26 && key->buf != tmk_KeyTab && key->buf != tmk_KeyRet) {
      key->buf = !key->buf ? 32 : key->buf + 96;
      key->mods |= tmk_ModCtrl;
    }
  end_l:
    if (wait) {
      setblk(1);
    }
    if (!ret && filt && !filt(key)) {
      continue;
    }
    if (wait) {
      filtsig(0, &sigbkp);
    } else {
      setblk(1);
    }
    setraw(0);
    break;
  }
#endif
  return ret;
}

void
tmk_getargs(int argc, const char **argv, struct tmk_args *a)
{
#ifdef _WIN32
  a->asutf16 = CommandLineToArgvW(GetCommandLineW(), &a->total);
  if (a->total == 1) {
    LocalFree(a->asutf16);
    a->total = 0;
    return;
  }
  ++a->asutf16;
  --a->total;
  a->asutf8 = malloc(sizeof(void*) * a->total);
  for (int i = 0; i < a->total; ++i) {
    a->asutf8[i] = tmk_asutf8(a->asutf16[i]);
  }
#else
  if (argc == 1) {
    a->total = 0;
    return;
  }
  a->total = argc - 1;
  a->asutf8 = argv + 1;
#endif
}

void
tmk_freeargs(struct tmk_args *a)
{
#ifdef _WIN32
  if (!a->total) {
    return;
  }
  LocalFree(a->asutf16 - 1);
  for (int i = 0; i < a->total; ++i) {
    free((void*)a->asutf8[i]);
  }
  free(a->asutf8);
#endif
}

void
tmk_vwrite(const char *fmt, va_list v)
{
  init();
  vprintf(fmt, v);
}

void
tmk_write(const char *fmt, ...)
{
  va_list v;
  va_start(v, fmt);
  tmk_vwrite(fmt, v);
  va_end(v);
}

void
tmk_vewrite(const char *fmt, va_list v)
{
  init();
  tmk_flushout();
  vfprintf(stderr, fmt, v);
}

void
tmk_ewrite(const char *fmt, ...)
{
  va_list v;
  va_start(v, fmt);
  tmk_vewrite(fmt, v);
  va_end(v);
}
