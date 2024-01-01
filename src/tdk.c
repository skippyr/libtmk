/* //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
 * BSD-3-Clause License
 * Copyright (c) 2023, Sherman Rofeman <skippyr.developer@gmail.com>
 *
 * See the LICENSE file that comes in its source code for more details.
 * //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\// */

#include "tdk.h"

#define PARSE_KEY_TRAIL(key, characterI, characterII, characterIII,            \
                        characterIV)                                           \
  if (keyTrail[0] == characterI && keyTrail[1] == characterII &&               \
      keyTrail[2] == characterIII && keyTrail[3] == characterIV) {             \
    keyHead = key;                                                             \
    goto reset;                                                                \
  }

static int clearInputBuffer(void);
static void setNonBlockMode(int isToEnable);
static int setRawMode(int isToEnable);
static int writeANSI(char *message);
static int writeFormattedANSI(size_t length, char *format, ...);

static int clearInputBuffer(void) {
  if (fwide(stdin, 0) < 0 || !setlocale(LC_ALL, "C.UTF-8") || setRawMode(1)) {
    return 1;
  }
  setNonBlockMode(1);
  while (getwchar() != WEOF)
    ;
  setRawMode(0);
  setNonBlockMode(0);
  return 0;
}

static void setNonBlockMode(int isToEnable) {
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL,
        isToEnable ? flags | O_NONBLOCK : flags & ~O_NONBLOCK);
}

static int setRawMode(int isToEnable) {
  struct termios attributes;
  if (tcgetattr(STDIN_FILENO, &attributes)) {
    return 1;
  }
  isToEnable ? (attributes.c_lflag &= ~(ICANON | ECHO))
             : (attributes.c_lflag |= ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  return 0;
}

static int writeANSI(char *message) {
  return !((isatty(STDOUT_FILENO) && printf("%s", message) >= 0 &&
            !fflush(stdout)) ||
           (isatty(STDERR_FILENO) && fprintf(stderr, "%s", message) >= 0));
}

static int writeFormattedANSI(size_t length, char *format, ...) {
  char *message;
  int result;
  va_list arguments;
  if (!(message = malloc(length))) {
    return 1;
  }
  va_start(arguments, format);
  vsnprintf(message, length, format, arguments);
  result = writeANSI(message);
  va_end(arguments);
  free(message);
  return result;
}

void tdk_clearCursorLine(void) {
  writeANSI("\33[2K\33[1G");
}

int tdk_getCursorPosition(int *column, int *line) {
  int temporaryColumn;
  int temporaryLine;
  if (clearInputBuffer() || setRawMode(1) || writeANSI("\33[6n")) {
    return 1;
  }
  wscanf(L"\33[%d;%dR", &temporaryLine, &temporaryColumn);
  if (column) {
    *column = --temporaryColumn;
  }
  if (line) {
    *line = --temporaryLine;
  }
  setRawMode(0);
  return 0;
}

int tdk_getScreenSize(int *columns, int *lines) {
  struct winsize size;
  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) &&
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) &&
      ioctl(STDERR_FILENO, TIOCGWINSZ, &size)) {
    return 1;
  }
  if (columns) {
    *columns = size.ws_col;
  }
  if (lines) {
    *lines = size.ws_row;
  }
  return 0;
}

int tdk_readKey(void) {
  int keyHead;
  int keyIndex;
  int keyTrail[4];
  if ((!isatty(STDOUT_FILENO) && !isatty(STDERR_FILENO)) ||
      clearInputBuffer()) {
    return WEOF;
  }
  setRawMode(1);
  keyHead = getwchar();
  setNonBlockMode(1);
  for (keyIndex = 0; keyIndex < 4; keyIndex++) {
    keyTrail[keyIndex] = getwchar();
  }
  if (keyHead == 27 && keyTrail[0] != WEOF) {
    for (keyIndex = 65; keyIndex < 69; keyIndex++) {
      PARSE_KEY_TRAIL(keyIndex - 65 + tdk_Key_ArrowUp, 91, keyIndex, WEOF,
                      WEOF);
    }
    for (keyIndex = 80; keyIndex < 84; keyIndex++) {
      PARSE_KEY_TRAIL(keyIndex - 80 + tdk_Key_F1, 79, keyIndex, WEOF, WEOF);
    }
    for (keyIndex = 65; keyIndex < 69; keyIndex++) {
      PARSE_KEY_TRAIL(keyIndex - 65 + tdk_Key_F1, 91, 91, keyIndex, WEOF);
    }
    PARSE_KEY_TRAIL(tdk_Key_F5, 91, 49, 53, 126);
    PARSE_KEY_TRAIL(tdk_Key_F5, 91, 91, 69, WEOF);
    for (keyIndex = 55; keyIndex < 58; keyIndex++) {
      PARSE_KEY_TRAIL(keyIndex - 55 + tdk_Key_F6, 91, 49, keyIndex, 126);
    }
    for (keyIndex = 48; keyIndex < 50; keyIndex++) {
      PARSE_KEY_TRAIL(keyIndex - 48 + tdk_Key_F9, 91, 50, keyIndex, 126);
    }
    for (keyIndex = 51; keyIndex < 53; keyIndex++) {
      PARSE_KEY_TRAIL(keyIndex - 51 + tdk_Key_F11, 91, 50, keyIndex, 126);
    }
    PARSE_KEY_TRAIL(tdk_Key_Insert, 91, 52, 104, WEOF);
    PARSE_KEY_TRAIL(tdk_Key_Insert, 91, 50, 126, WEOF);
    PARSE_KEY_TRAIL(tdk_Key_Delete, 91, 80, WEOF, WEOF);
    PARSE_KEY_TRAIL(tdk_Key_Delete, 91, 51, 126, WEOF);
    PARSE_KEY_TRAIL(tdk_Key_End, 91, 52, 126, WEOF);
    PARSE_KEY_TRAIL(tdk_Key_End, 91, 70, WEOF, WEOF);
    PARSE_KEY_TRAIL(tdk_Key_Home, 91, 72, WEOF, WEOF);
    PARSE_KEY_TRAIL(tdk_Key_Home, 91, 49, 126, WEOF);
    for (keyIndex = 53; keyIndex < 55; keyIndex++) {
      PARSE_KEY_TRAIL(keyIndex - 53 + tdk_Key_PageUp, 91, keyIndex, 126, WEOF);
    }
    keyHead = 0;
  }
reset:
  clearInputBuffer();
  return keyHead < 0 || keyHead == 9 || keyHead == 10 || keyHead > 26 ? keyHead
                                                                      : 0;
}

void tdk_ringBell(void) {
  writeANSI("\7");
}

void tdk_setAlternateScreen(int isToOpen) {
  writeANSI(isToOpen ? "\33[?1049h\33[2J\33[1;1H" : "\33[?1049l");
}

void tdk_setColor(int layer, int color) {
  writeFormattedANSI(6, "\33[%d%dm", layer, color);
}

void tdk_setColorBrightness(int brightness) {
  writeFormattedANSI(8, !brightness ? "\33[22m" : "\33[22;%dm", brightness);
}

int tdk_setCursorPosition(int column, int line) {
  int screenColumns;
  int screenLines;
  return tdk_getScreenSize(&screenColumns, &screenLines) || ++column < 1 ||
         column > screenColumns || ++line < 1 || line > screenLines ||
         writeFormattedANSI(13, "\33[%d;%dH", line, column);
}

void tdk_setCursorShape(int shape) {
  writeFormattedANSI(6, "\33[%d q", shape);
}

void tdk_setCursorVisibility(int isToMakeVisible) {
  writeANSI(isToMakeVisible ? "\33[?25h" : "\33[?25l");
}

void tdk_setTextEffect(int effect, int isToEnable) {
  int ansi;
  for (ansi = 3; ansi < 10; ansi++) {
    if (1 << ansi & effect) {
      writeFormattedANSI(6, "\33[%dm", isToEnable ? ansi : ansi + 20);
    }
  }
}
