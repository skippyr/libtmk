#include "tmk.hh"
#if defined(_WIN32)
#include <Windows.h>
#include <io.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif
#include <inttypes.h>

#if defined(_WIN32)
#define IS_STREAM_REDIRECTED(stream_a) (!_isatty(stream_a) << stream_a)
#else
#define IS_STREAM_REDIRECTED(stream_a) (!isatty(stream_a) << stream_a)
#endif

namespace tmk {
RgbColor::RgbColor(uint8_t red, uint8_t green, uint8_t blue) noexcept
    : red_m(red), green_m(green), blue_m(blue) {}

uint8_t RgbColor::getRed() const noexcept { return red_m; }

void RgbColor::setRed(uint8_t red) noexcept { red_m = red; }

uint8_t RgbColor::getGreen() const noexcept { return green_m; }

void RgbColor::setGreen(uint8_t green) noexcept { green_m = green; }

uint8_t RgbColor::getBlue() const noexcept { return blue_m; }

void RgbColor::setBlue(uint8_t blue) noexcept { blue_m = blue; }

Coordinate::Coordinate(uint16_t column, uint16_t row) noexcept
    : column_m(column), row_m(row) {}

uint16_t Coordinate::getColumn() const noexcept { return column_m; }

void Coordinate::setColumn(uint16_t column) noexcept { column_m = column; }

uint16_t Coordinate::getRow() const noexcept { return row_m; }

void Coordinate::setRow(uint16_t row) noexcept { row_m = row; }

Dimensions::Dimensions(uint16_t columns, uint16_t rows) noexcept
    : columns_m(columns), rows_m(rows) {}

uint16_t Dimensions::getColumns() const noexcept { return columns_m; }

uint16_t Dimensions::getRows() const noexcept { return rows_m; }

uint32_t Dimensions::getArea() const noexcept { return columns_m * rows_m; }

uint8_t Terminal::cache_m = 0;

#if defined(_WIN32)
void Terminal::enableAnsiParse() noexcept {
  HANDLE handle;
  DWORD mode;
  (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
   GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
      SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
void Terminal::setRawInput(bool isRaw) noexcept {
  struct termios attributes;
  tcgetattr(STDIN_FILENO, &attributes);
  attributes.c_lflag = isRaw ? attributes.c_lflag & ~(ICANON | ECHO | ISIG)
                             : attributes.c_lflag | ICANON | ECHO | ISIG;
  attributes.c_iflag =
      isRaw ? attributes.c_iflag & ~IXON : attributes.c_iflag | IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
}

void Terminal::setBlockingInput(bool isBlocking) noexcept {
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL,
        isBlocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
}
#endif

void Terminal::cacheStreamStates() noexcept {
  cache_m |= IS_STREAM_REDIRECTED(0) | IS_STREAM_REDIRECTED(1) |
             IS_STREAM_REDIRECTED(2);
}

void Terminal::init() noexcept {
  if (cache_m & 1 << 7) {
    return;
  }
  cache_m |= 1 << 7;
#if defined(_WIN32)
  SetConsoleOutputCP(CP_UTF8);
  enableAnsiParse();
#endif
  cacheStreamStates();
}

void Terminal::flushOutput() noexcept { std::fflush(stdout); }

void Terminal::clearInput() noexcept {
#if defined(_WIN32)
  FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
  if (isInputRedirected()) {
    return;
  }
  setRawInput(true);
  setBlockingInput(false);
  while (std::getchar() != EOF) {
  }
  setBlockingInput(true);
  setRawInput(false);
#endif
}

bool Terminal::isInputRedirected() noexcept {
  init();
  return cache_m & 1;
}

bool Terminal::isOutputRedirected() noexcept {
  init();
  return cache_m & 1 << 1;
}

bool Terminal::isErrorRedirected() noexcept {
  init();
  return cache_m & 1 << 2;
}

void Terminal::setFontColor(AnsiColor color, Layer layer) noexcept {
  writeAnsi("\x1b[{}8;5;{}m", static_cast<int>(layer), static_cast<int>(color));
}

void Terminal::setFontColor(const RgbColor &color, Layer layer) noexcept {
  writeAnsi("\x1b[{}8;2;{};{};{}m", static_cast<int>(layer), color.getRed(),
            color.getGreen(), color.getBlue());
}

void Terminal::resetFontColors() noexcept { writeAnsi("\x1b[39;49m"); }

void Terminal::setFontWeight(FontWeight weight) noexcept {
  writeAnsi("\x1b[22;{}m", static_cast<int>(weight));
}

void Terminal::resetFontWeight() noexcept { writeAnsi("\x1b[22m"); }

void Terminal::setCursorVisible(bool isVisible) noexcept {
  writeAnsi("\x1b[?25{}", isVisible ? 'h' : 'l');
}

void Terminal::setCursorShape(CursorShape shape, bool shouldBlink) noexcept {
  writeAnsi("\x1b[{} q", static_cast<int>(shape) - shouldBlink);
}

void Terminal::resetCursorShape() noexcept { writeAnsi("\x1b[0 q"); }

Coordinate Terminal::getCursorCoordinate() {
#if defined(_WIN32)
  CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                  &bufferInfo) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                  &bufferInfo)) {
    throw StreamRedirectionException();
  }
  return Coordinate(bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left,
                    bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top);
#else
  if (isInputRedirected() || (isOutputRedirected() && isErrorRedirected())) {
    throw StreamRedirectionException();
  }
  clearInput();
  setRawInput(true);
  writeAnsi("\x1b[6n");
  uint16_t column;
  uint16_t row;
  scanf("\x1b[%" PRIu16 ";%" PRIu16 "R", &row, &column);
  setRawInput(false);
  return Coordinate(column - 1, row - 1);
#endif
}

Dimensions Terminal::getWindowDimensions() {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
  if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                  &bufferInfo) &&
      !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                  &bufferInfo)) {
    throw StreamRedirectionException();
  }
  return Dimensions(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1,
                    bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1)
#else
  struct winsize ioctlSize;
  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ioctlSize) &&
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &ioctlSize) &&
      ioctl(STDERR_FILENO, TIOCGWINSZ, &ioctlSize)) {
    throw StreamRedirectionException();
  }
  return Dimensions(ioctlSize.ws_col, ioctlSize.ws_row);
#endif
}

void Terminal::clearWindow() noexcept { writeAnsi("\x1b[2J\x1b[1;1H"); }

void Terminal::clearLine() noexcept { writeAnsi("\x1b[2K\x1b[1G"); }

void Terminal::ringBell() noexcept { writeAnsi("\7"); }

void Terminal::openAlternateWindow() noexcept {
  writeAnsi("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

void Terminal::closeAlternateWindow() noexcept { writeAnsi("\x1b[?1049l"); }

void Terminal::writeLine() noexcept {
  init();
  cache_m &= ~(1 << 4);
  try {
    std::cout << std::endl;
  } catch (...) {
  }
}

void Terminal::writeErrorLine() noexcept {
  init();
  if (cache_m & 1 << 4) {
    cache_m &= ~(1 << 4);
    flushOutput();
  }
  try {
    std::cerr << std::endl;
  } catch (...) {
  }
}
} // namespace tmk
