#include "tmk.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
#include <Windows.h>
#include <io.h>
#else
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#endif

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
#define _tmk_REDIRECTION_CACHE(stream_a) (!_isatty(stream_a) << stream_a)
#else
#define _tmk_REDIRECTION_CACHE(stream_a) (!isatty(stream_a) << stream_a)
#define _tmk_TIMESPEC_TO_MILLISECONDS(timespec_a)                              \
  (timespec_a.tv_sec * 1000 + timespec_a.tv_nsec / 1000000)
#endif
#if tmk_IS_OPERATING_SYSTEM_MACOS
#define _tmk_PARSE_OPTION_KEY(value_a, key_a)                                  \
  case value_a:                                                                \
    temporaryEvent.key = key_a;                                                \
    temporaryEvent.modifiers |= tmk_ModifierKey_AltOption;                     \
    goto reset_l;
#endif
#define _tmk_PARSE_KEY(condition_a, key_a)                                     \
  if (condition_a) {                                                           \
    temporaryEvent.key = key_a;                                                \
    break;                                                                     \
  }
#define _tmk_IS_REDIRECTED(stream_a) (_tmk_redirectionCache_g & 1 << stream_a)
#define _tmk_HAS_REDIRECTION_CACHE_FLAG (1 << 7)
#define _tmk_MAXIMUM(numberI_a, numberII_a)                                    \
  (numberI_a > numberII_a ? numberI_a : numberII_a)
#define _tmk_SIGNED_EOF 255

static char _tmk_redirectionCache_g = 0;

#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
static void _tmk_handleSIGWINCH(int signal);
#endif
static void _tmk_initRedirectionCache(void);
static int _tmk_writeANSISequence(const char *format, ...);

#if !tmk_IS_OPERATING_SYSTEM_WINDOWS
static void _tmk_handleSIGWINCH(int signal) {}
#endif

static void _tmk_initRedirectionCache(void) {
  if (_tmk_redirectionCache_g & _tmk_HAS_REDIRECTION_CACHE_FLAG) {
    return;
  }
  _tmk_redirectionCache_g = _tmk_REDIRECTION_CACHE(tmk_Stream_Input) |
                            _tmk_REDIRECTION_CACHE(tmk_Stream_Output) |
                            _tmk_REDIRECTION_CACHE(tmk_Stream_Error) |
                            _tmk_HAS_REDIRECTION_CACHE_FLAG;
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  SetConsoleOutputCP(CP_UTF8);
  HANDLE handle;
  DWORD mode;
  (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
   GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
      SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

static int _tmk_writeANSISequence(const char *format, ...) {
  _tmk_initRedirectionCache();
  if (_tmk_IS_REDIRECTED(tmk_Stream_Output) &&
      _tmk_IS_REDIRECTED(tmk_Stream_Error)) {
    return -1;
  }
  va_list arguments;
  va_start(arguments, format);
  vfprintf(!_tmk_IS_REDIRECTED(tmk_Stream_Output) ? stdout : stderr, format,
           arguments);
  va_end(arguments);
  return 0;
}

#if tmk_IS_OPERATING_SYSTEM_WINDOWS
char *tmk_convertUTF16ToUTF8(const wchar_t *utf16String, size_t *length) {
  int temporaryLength =
      WideCharToMultiByte(CP_UTF8, 0, utf16String, -1, NULL, 0, NULL, NULL);
  char *utf8String = malloc(temporaryLength);
  WideCharToMultiByte(CP_UTF8, 0, utf16String, -1, utf8String, temporaryLength,
                      NULL, NULL);
  if (length) {
    *length = temporaryLength - 1;
  }
  return utf8String;
}

wchar_t *tmk_convertUTF8ToUTF16(const char *utf8String, size_t *length) {
  int temporaryLength =
      MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, NULL, 0);
  wchar_t *utf16String = malloc(temporaryLength * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, utf16String, temporaryLength);
  if (length) {
    *length = temporaryLength - 1;
  }
  return utf16String;
}
#endif

int tmk_isStreamRedirected(enum tmk_Stream stream) {
  _tmk_initRedirectionCache();
  return _tmk_IS_REDIRECTED(stream);
}

void tmk_flushOutputBuffer(void) { fflush(stdout); }

void tmk_clearInputBuffer(void) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
  struct termios attributes;
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  tcgetattr(STDIN_FILENO, &attributes);
  attributes.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
  while (getchar() != EOF) {
  }
  attributes.c_lflag |= ICANON | ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  fcntl(STDIN_FILENO, F_SETFL, flags);
#endif
}

void tmk_ringBell(void) { _tmk_writeANSISequence("\7"); }

void tmk_clearCursorLine(void) { _tmk_writeANSISequence("\x1b[2K\x1b[1G"); }

void tmk_getCMDArguments(int totalRawCMDArguments, const char **rawCMDArguments,
                         struct tmk_CMDArguments *cmdArguments) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  cmdArguments->utf16Arguments =
      CommandLineToArgvW(GetCommandLineW(), &cmdArguments->totalArguments);
  cmdArguments->utf8Arguments =
      malloc(cmdArguments->totalArguments * sizeof(const char **));
  for (int offset = 0; offset < cmdArguments->totalArguments; ++offset) {
    cmdArguments->utf8Arguments[offset] =
        tmk_convertUTF16ToUTF8(cmdArguments->utf16Arguments[offset], NULL);
  }
#else
  cmdArguments->utf8Arguments = rawCMDArguments;
  cmdArguments->totalArguments = totalRawCMDArguments;
#endif
}

void tmk_freeCMDArguments(struct tmk_CMDArguments *arguments) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  LocalFree(arguments->utf16Arguments);
  for (int offset = 0; offset < arguments->totalArguments; ++offset) {
    free((void *)arguments->utf8Arguments[offset]);
  }
#endif
}

int tmk_getWindowDimensions(struct tmk_Dimensions *dimensions) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                  &bufferInfo) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                  &bufferInfo)) {
    return -1;
  }
  dimensions->totalColumns =
      bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
  dimensions->totalRows =
      bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;
#else
  struct winsize size;
  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) &&
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) &&
      ioctl(STDERR_FILENO, TIOCGWINSZ, &size)) {
    return -1;
  }
  dimensions->totalColumns = size.ws_col;
  dimensions->totalRows = size.ws_row;
#endif
  dimensions->area = dimensions->totalColumns * dimensions->totalRows;
  return 0;
}

void tmk_setFontANSIColor(unsigned char color, enum tmk_Layer layer) {
  _tmk_writeANSISequence("\x1b[%d8;5;%dm", layer, color);
}

void tmk_setFontRGBColor(struct tmk_RGBColor color, enum tmk_Layer layer) {
  _tmk_writeANSISequence("\x1b[%d8;2;%hu;%hu;%hum", layer, color.red,
                         color.green, color.blue);
}

void tmk_resetFontColors(void) { _tmk_writeANSISequence("\x1b[39;49m"); }

void tmk_setFontWeight(enum tmk_FontWeight weight) {
  _tmk_writeANSISequence("\x1b[22;%dm", weight);
}

void tmk_resetFontWeight(void) { _tmk_writeANSISequence("\x1b[22m"); }

void tmk_setFontEffects(int effectsMask) {
  for (int effect = 3; effect < 10; ++effect) {
    if (effectsMask & 1 << effect) {
      _tmk_writeANSISequence("\x1b[%dm", effect);
    }
  }
}

void tmk_resetFontEffects(void) {
  for (int effect = 23; effect < 30; ++effect) {
    if (effect != 26) {
      _tmk_writeANSISequence("\x1b[%dm", effect);
    }
  }
}

void tmk_openAlternateWindow(void) {
  _tmk_writeANSISequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

void tmk_closeAlternateWindow(void) { _tmk_writeANSISequence("\x1b[?1049l"); }

int tmk_getCursorCoordinate(struct tmk_Coordinate *coordinate) {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                  &bufferInfo) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                  &bufferInfo)) {
    return -1;
  }
  coordinate->column = bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left;
  coordinate->row = bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Right;
#else
  tmk_clearInputBuffer();
  struct termios attributes;
  if (_tmk_writeANSISequence("\x1b[6n") ||
      tcgetattr(STDIN_FILENO, &attributes)) {
    return -1;
  }
  attributes.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  int totalMatches =
      scanf("\x1b[%hu;%huR", &coordinate->row, &coordinate->column);
  attributes.c_lflag |= ICANON | ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  if (totalMatches != 2) {
    return -1;
  }
  --coordinate->row;
  --coordinate->column;
#endif
  return 0;
}

void tmk_setCursorCoordinate(struct tmk_Coordinate coordinate) {
  _tmk_writeANSISequence("\x1b[%hu;%huH", coordinate.row + 1,
                         coordinate.column + 1);
}

void tmk_setCursorShape(enum tmk_CursorShape shape, int isBlinking) {
  _tmk_writeANSISequence("\x1b[%d q", shape - !!isBlinking);
}

void tmk_resetCursorShape(void) { _tmk_writeANSISequence("\x1b[0 q"); }

void tmk_setCursorVisible(int isVisible) {
  _tmk_writeANSISequence("\x1b[?25%c", isVisible ? 'h' : 'l');
}

int tmk_readKeyEvent(short waitInMilliseconds, struct tmk_KeyEvent *event,
                     int (*filter)(struct tmk_KeyEvent *)) {
  _tmk_initRedirectionCache();
  if (_tmk_IS_REDIRECTED(tmk_Stream_Input) || fwide(stdin, 0) > 0 ||
      (_tmk_IS_REDIRECTED(tmk_Stream_Output) &&
       _tmk_IS_REDIRECTED(tmk_Stream_Error))) {
    return -1;
  }
  struct tmk_KeyEvent temporaryEvent;
  tmk_flushOutputBuffer();
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
  HANDLE timer = NULL;
  DWORD mode;
  GetConsoleMode(input, &mode);
  SetConsoleMode(input, mode & ~ENABLE_PROCESSED_INPUT);
parse_l:
  int returnCode = 0;
  while (1) {
    if (!waitInMilliseconds) {
      DWORD totalEvents;
      GetNumberOfConsoleInputEvents(input, &totalEvents);
      if (!totalEvents) {
        returnCode = -2;
        break;
      }
    } else if (waitInMilliseconds > 0) {
      if (!timer) {
        timer = CreateWaitableTimerW(NULL, TRUE, NULL);
        LARGE_INTEGER dueTime;
        dueTime.QuadPart = waitInMilliseconds * -10000;
        SetWaitableTimer(timer, &dueTime, 0, NULL, NULL, FALSE);
      }
      HANDLE objects[] = {timer, input};
      if (WaitForMultipleObjects(2, objects, FALSE, INFINITE) ==
          WAIT_OBJECT_0) {
        returnCode = -3;
        break;
      }
    }
    INPUT_RECORD record;
    DWORD totalEventsRead;
    ReadConsoleInputW(input, &record, 1, &totalEventsRead);
    if (record.EventType == WINDOW_BUFFER_SIZE_EVENT) {
      returnCode = -4;
      break;
    }
    if (record.EventType != KEY_EVENT || !record.Event.KeyEvent.bKeyDown ||
        record.Event.KeyEvent.wVirtualKeyCode == VK_CONTROL ||
        record.Event.KeyEvent.wVirtualKeyCode == VK_SHIFT ||
        record.Event.KeyEvent.wVirtualKeyCode == VK_MENU ||
        record.Event.KeyEvent.wVirtualKeyCode == VK_CAPITAL ||
        record.Event.KeyEvent.wVirtualKeyCode == VK_NUMLOCK ||
        record.Event.KeyEvent.wVirtualKeyCode == VK_SCROLL) {
      continue;
    }
    memset(&temporaryEvent, 0, sizeof(struct tmk_KeyEvent));
    int buffer = record.Event.KeyEvent.uChar.UnicodeChar;
    if (buffer) {
      if (buffer <= 26 && buffer != tmk_Key_Tab && buffer != tmk_Key_Enter) {
        temporaryEvent.key = buffer + 96;
      } else if (buffer >= HIGH_SURROGATE_START &&
                 buffer <= HIGH_SURROGATE_END) {
        ReadConsoleInputW(input, &record, 1, &totalEventsRead);
        ReadConsoleInputW(input, &record, 1, &totalEventsRead);
        *((short *)&buffer + 1) = record.Event.KeyEvent.uChar.UnicodeChar;
        WideCharToMultiByte(CP_UTF8, 0, (wchar_t *)&buffer, 2,
                            (char *)&temporaryEvent.key, 4, NULL, NULL);
      } else {
        WideCharToMultiByte(CP_UTF8, 0, (wchar_t *)&buffer, 1,
                            (char *)&temporaryEvent.key, 4, NULL, NULL);
      }
      temporaryEvent.modifiers = !!(record.Event.KeyEvent.dwControlKeyState &
                                    (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) |
                                 !!(record.Event.KeyEvent.dwControlKeyState &
                                    (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
                                     << 1;
      break;
    } else if (record.Event.KeyEvent.dwControlKeyState &
               (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED | LEFT_CTRL_PRESSED |
                RIGHT_CTRL_PRESSED | SHIFT_PRESSED)) {
      continue;
    }
    _tmk_PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_LEFT &&
                       record.Event.KeyEvent.wVirtualKeyCode <= VK_DOWN,
                   record.Event.KeyEvent.wVirtualKeyCode - VK_LEFT +
                       tmk_Key_LeftArrow);
    _tmk_PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_PRIOR &&
                       record.Event.KeyEvent.wVirtualKeyCode <= VK_HOME,
                   record.Event.KeyEvent.wVirtualKeyCode - VK_PRIOR +
                       tmk_Key_PageUp);
    _tmk_PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_INSERT &&
                       record.Event.KeyEvent.wVirtualKeyCode <= VK_DELETE,
                   record.Event.KeyEvent.wVirtualKeyCode - VK_INSERT +
                       tmk_Key_Insert);
    _tmk_PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_F1 &&
                       record.Event.KeyEvent.wVirtualKeyCode <= VK_F12,
                   record.Event.KeyEvent.wVirtualKeyCode - VK_F1 + tmk_Key_F1);
  }
  if (waitInMilliseconds > 0 && !returnCode && filter &&
      !filter(&temporaryEvent)) {
    goto parse_l;
  }
  if (timer) {
    CloseHandle(timer);
  }
  SetConsoleMode(input, mode);
#else
  sigset_t blockedSignals;
  sigset_t backupSignals;
  sigfillset(&blockedSignals);
  sigdelset(&blockedSignals, SIGWINCH);
  pthread_sigmask(SIG_SETMASK, &blockedSignals, &backupSignals);
  struct sigaction sigwinchAction;
  sigwinchAction.sa_handler = _tmk_handleSIGWINCH;
  sigwinchAction.sa_flags = 0;
  sigemptyset(&sigwinchAction.sa_mask);
  sigaction(SIGWINCH, &sigwinchAction, NULL);
  struct termios attributes;
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  tcgetattr(STDIN_FILENO, &attributes);
  attributes.c_lflag &= ~(ICANON | ECHO | ISIG);
  attributes.c_iflag &= ~IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  long timer = waitInMilliseconds;
  int returnCode;
parse_l:
  returnCode = 0;
  while (1) {
    struct pollfd stdinPollFd = {STDIN_FILENO, POLLIN, 0};
    unsigned char buffer[5];
    memset(buffer, 0, sizeof(buffer));
    if (!waitInMilliseconds) {
      fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
      if ((buffer[0] = getchar()) == _tmk_SIGNED_EOF) {
        returnCode = -2;
        break;
      }
    } else if (waitInMilliseconds > 0 && timer >= 0) {
      fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
      if ((buffer[0] = getchar()) == _tmk_SIGNED_EOF) {
        struct timespec initTime;
        struct timespec endTime;
        clock_gettime(CLOCK_MONOTONIC, &initTime);
        int status = poll(&stdinPollFd, 1, timer);
        clock_gettime(CLOCK_MONOTONIC, &endTime);
        timer = _tmk_MAXIMUM(0, timer - _tmk_TIMESPEC_TO_MILLISECONDS(endTime) +
                                    _tmk_TIMESPEC_TO_MILLISECONDS(initTime));
        if (status <= 0) {
          returnCode = status < 0 && errno == EINTR ? -4 : -3;
          break;
        }
        buffer[0] = getchar();
      }
    } else {
      buffer[0] = getchar();
      fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }
    memset(&temporaryEvent, 0, sizeof(struct tmk_KeyEvent));
#if tmk_IS_OPERATING_SYSTEM_MACOS
    switch (buffer[0]) {
      _tmk_PARSE_OPTION_KEY(38, 'J');
      _tmk_PARSE_OPTION_KEY(47, 'q');
      _tmk_PARSE_OPTION_KEY(60, 'Z');
      _tmk_PARSE_OPTION_KEY(62, 'X');
      _tmk_PARSE_OPTION_KEY(63, 'w');
      _tmk_PARSE_OPTION_KEY(92, 'Q');
    case 194:
      buffer[1] = getchar();
      switch (buffer[1]) {
        _tmk_PARSE_OPTION_KEY(167, 'S');
        _tmk_PARSE_OPTION_KEY(169, 'C');
        _tmk_PARSE_OPTION_KEY(174, 'r');
        _tmk_PARSE_OPTION_KEY(181, 'm');
        _tmk_PARSE_OPTION_KEY(191, 'W');
      }
    case 195:
      buffer[1] = getchar();
      switch (buffer[1]) {
        _tmk_PARSE_OPTION_KEY(134, 'A');
        _tmk_PARSE_OPTION_KEY(144, 'D');
        _tmk_PARSE_OPTION_KEY(152, 'O');
        _tmk_PARSE_OPTION_KEY(158, 'P');
        _tmk_PARSE_OPTION_KEY(159, 's');
        _tmk_PARSE_OPTION_KEY(166, 'a');
        _tmk_PARSE_OPTION_KEY(176, 'd');
        _tmk_PARSE_OPTION_KEY(184, 'o');
        _tmk_PARSE_OPTION_KEY(190, 'p');
      }
    case 196:
      buffer[1] = getchar();
      switch (buffer[1]) {
        _tmk_PARSE_OPTION_KEY(145, 'f');
        _tmk_PARSE_OPTION_KEY(166, 'H');
        _tmk_PARSE_OPTION_KEY(167, 'h');
        _tmk_PARSE_OPTION_KEY(177, 'I');
        _tmk_PARSE_OPTION_KEY(184, 'k');
      }
    case 197:
      buffer[1] = getchar();
      switch (buffer[1]) {
        _tmk_PARSE_OPTION_KEY(129, 'L');
        _tmk_PARSE_OPTION_KEY(130, 'l');
        _tmk_PARSE_OPTION_KEY(138, 'N');
        _tmk_PARSE_OPTION_KEY(139, 'n');
        _tmk_PARSE_OPTION_KEY(166, 'T');
        _tmk_PARSE_OPTION_KEY(167, 't');
      }
    case 198:
      buffer[1] = getchar();
      switch (buffer[1]) { _tmk_PARSE_OPTION_KEY(178, 'V'); }
    case 202:
      buffer[1] = getchar();
      switch (buffer[1]) {
        _tmk_PARSE_OPTION_KEY(139, 'v');
        _tmk_PARSE_OPTION_KEY(157, 'j');
      }
    case 203:
      buffer[1] = getchar();
      switch (buffer[1]) { _tmk_PARSE_OPTION_KEY(157, 'G'); }
    case 206:
      buffer[1] = getchar();
      switch (buffer[1]) { _tmk_PARSE_OPTION_KEY(169, 'z'); }
    case 226:
      buffer[1] = getchar();
      switch (buffer[1]) {
      case 130:
        buffer[2] = getchar();
        switch (buffer[2]) {
          _tmk_PARSE_OPTION_KEY(162, 'c');
          _tmk_PARSE_OPTION_KEY(172, 'e');
        }
      case 132:
        buffer[2] = getchar();
        switch (buffer[2]) { _tmk_PARSE_OPTION_KEY(162, 'B'); }
      case 134:
        buffer[2] = getchar();
        switch (buffer[2]) {
          _tmk_PARSE_OPTION_KEY(144, 'y');
          _tmk_PARSE_OPTION_KEY(145, 'U');
          _tmk_PARSE_OPTION_KEY(146, 'i');
          _tmk_PARSE_OPTION_KEY(147, 'u');
        }
      case 136:
        buffer[2] = getchar();
        switch (buffer[2]) {
          _tmk_PARSE_OPTION_KEY(134, 'g');
          _tmk_PARSE_OPTION_KEY(171, 'b');
        }
      case 137:
        buffer[2] = getchar();
        switch (buffer[2]) { _tmk_PARSE_OPTION_KEY(136, 'x'); }
      case 151:
        buffer[2] = getchar();
        switch (buffer[2]) { _tmk_PARSE_OPTION_KEY(138, 'F'); }
      }
    case 239:
      buffer[1] = getchar();
      switch (buffer[1]) {
      case 163:
        buffer[2] = getchar();
        switch (buffer[2]) { _tmk_PARSE_OPTION_KEY(191, 'K'); }
      case 157:
        buffer[2] = getchar();
        if (buffer[2] == 134) {
          event->key = tmk_Key_Insert;
          goto reset_l;
        }
      }
    }
#endif
    if (buffer[0] == 27 && ((buffer[1] = getchar()) == 91 || buffer[1] == 79)) {
      for (int offset = 2; offset < sizeof(buffer); ++offset) {
        buffer[offset] = getchar();
      }
      while (getchar() != EOF) {
      }
      _tmk_PARSE_KEY(buffer[2] >= 65 && buffer[2] <= 68,
                     buffer[2] - 65 + tmk_Key_UpArrow);
      _tmk_PARSE_KEY(buffer[3] == 126, buffer[2] - 49 + tmk_Key_Home);
      _tmk_PARSE_KEY(buffer[3] == 104 || (buffer[1] == 91 && buffer[2] == 80),
                     !(buffer[3] == 104) + tmk_Key_Home);
      _tmk_PARSE_KEY(buffer[2] == 70 || buffer[2] == 72,
                     buffer[2] == 72 ? tmk_Key_Home : tmk_Key_End);
      _tmk_PARSE_KEY(buffer[2] >= 80 && buffer[2] <= 83,
                     buffer[2] - 80 + tmk_Key_F1);
      _tmk_PARSE_KEY(buffer[3] >= 65 && buffer[3] <= 69,
                     buffer[3] - 65 + tmk_Key_F1);
      _tmk_PARSE_KEY(buffer[4] == 126, buffer[3] == 53 ? tmk_Key_F5
                                       : buffer[3] >= 55 && buffer[3] <= 57
                                           ? buffer[3] - 55 + tmk_Key_F6
                                       : buffer[3] == 48 || buffer[3] == 49
                                           ? buffer[3] - 48 + tmk_Key_F9
                                           : buffer[3] - 51 + tmk_Key_F11);
      fcntl(STDIN_FILENO, F_SETFL, flags);
      continue;
    } else if (buffer[0] & 1 << 7) {
      for (int offset = 1;
           offset < 1 + !!(buffer[0] & 1 << 6) + !!(buffer[0] & 1 << 5) +
                        !!(buffer[0] & 1 << 4);
           ++offset) {
        buffer[offset] = getchar();
      }
      temporaryEvent.key = *(int *)buffer;
    } else if ((temporaryEvent.key =
                    (temporaryEvent.modifiers =
                         buffer[0] == 27 && buffer[1] != _tmk_SIGNED_EOF)
                        ? buffer[1]
                        : buffer[0]) >= 0 &&
               temporaryEvent.key <= 26 && temporaryEvent.key != tmk_Key_Tab &&
               temporaryEvent.key != tmk_Key_Enter) {
      temporaryEvent.key =
          !temporaryEvent.key ? tmk_Key_Space : temporaryEvent.key + 96;
      temporaryEvent.modifiers |= tmk_ModifierKey_Ctrl;
    }
    break;
  }
reset_l:
  fcntl(STDIN_FILENO, F_SETFL, flags);
  if (waitInMilliseconds > 0 && !returnCode && filter &&
      !filter(&temporaryEvent)) {
    goto parse_l;
  }
  attributes.c_lflag |= ICANON | ECHO | ISIG;
  attributes.c_iflag |= IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  sigwinchAction.sa_handler = SIG_DFL;
  sigaction(SIGWINCH, &sigwinchAction, NULL);
  pthread_sigmask(SIG_SETMASK, &backupSignals, NULL);
#endif
  if (event && !returnCode) {
    *event = temporaryEvent;
  }
  return returnCode;
}

void tmk_writeArguments(const char *format, va_list arguments) {
  _tmk_initRedirectionCache();
  vprintf(format, arguments);
}

void tmk_writeArgumentsLine(const char *format, va_list arguments) {
  tmk_writeArguments(format, arguments);
  putchar('\n');
}

void tmk_writeErrorArguments(const char *format, va_list arguments) {
  _tmk_initRedirectionCache();
  tmk_flushOutputBuffer();
  vfprintf(stderr, format, arguments);
}

void tmk_writeErrorArgumentsLine(const char *format, va_list arguments) {
  tmk_writeErrorArguments(format, arguments);
  fputc('\n', stderr);
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
