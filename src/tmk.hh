#pragma once

#include <format>
#include <iostream>

namespace tmk {
enum class Layer { Foreground = 3, Background };

enum class AnsiColor {
  DarkBlack,
  DarkRed,
  DarkGreen,
  DarkYellow,
  DarkBlue,
  DarkMagenta,
  DarkCyan,
  DarkWhite,
  LightBlack,
  LightRed,
  LightGreen,
  LightYellow,
  LightBlue,
  LightMagenta,
  LightCyan,
  LightWhite
};

enum class FontWeight { Bold = 1, Light };

enum class CursorShape { Block = 2, Underline = 4, Bar = 6 };

class StreamRedirectionException : public std::exception {};
class OutOfBoundsException : public std::exception {};

class RgbColor {
private:
  uint8_t red_m;
  uint8_t green_m;
  uint8_t blue_m;

public:
  RgbColor(uint8_t red, uint8_t green, uint8_t blue) noexcept;
  uint8_t getRed() const noexcept;
  void setRed(uint8_t red) noexcept;
  uint8_t getGreen() const noexcept;
  void setGreen(uint8_t green) noexcept;
  uint8_t getBlue() const noexcept;
  void setBlue(uint8_t blue) noexcept;
};

class Coordinate {
private:
  uint16_t column_m;
  uint16_t row_m;

public:
  Coordinate(uint16_t column, uint16_t row) noexcept;
  uint16_t getColumn() const noexcept;
  void setColumn(uint16_t column) noexcept;
  uint16_t getRow() const noexcept;
  void setRow(uint16_t row) noexcept;
};

class Dimensions {
private:
  uint16_t columns_m;
  uint16_t rows_m;

public:
  Dimensions(uint16_t columns, uint16_t rows) noexcept;
  uint16_t getColumns() const noexcept;
  uint16_t getRows() const noexcept;
  uint32_t getArea() const noexcept;
};

class Terminal {
private:
  static uint8_t cache_m;

#if defined(_WIN32)
  static void enableAnsiParse() noexcept;
#else
  static void setRawInput(bool isRaw) noexcept;
  static void setBlockingInput(bool isBlocking) noexcept;
#endif
  static void cacheStreamStates() noexcept;

  template <typename... Args>
  static void writeAnsi(std::string_view format, Args... arguments) {
    if (!isOutputRedirected()) {
      write(format, arguments...);
      cache_m |= 1 << 4;
    } else if (!isErrorRedirected()) {
      writeError(format, arguments...);
    }
  }

public:
  static void init() noexcept;
  static void flushOutput() noexcept;
  static void clearInput() noexcept;
  static bool isInputRedirected() noexcept;
  static bool isOutputRedirected() noexcept;
  static bool isErrorRedirected() noexcept;
  static void setFontColor(AnsiColor color, Layer layer) noexcept;
  static void setFontColor(const RgbColor &color, Layer layer) noexcept;
  static void resetFontColors() noexcept;
  static void setFontWeight(FontWeight weight) noexcept;
  static void resetFontWeight() noexcept;
  static void setCursorVisible(bool isVisible) noexcept;
  static void setCursorShape(CursorShape shape, bool shouldBlink) noexcept;
  static void resetCursorShape() noexcept;
  static Coordinate getCursorCoordinate();
  static void setCursorCoordinate(Coordinate coordinate);
  static Dimensions getWindowDimensions();
  static void clearWindow() noexcept;
  static void clearLine() noexcept;
  static void ringBell() noexcept;
  static void openAlternateWindow() noexcept;
  static void closeAlternateWindow() noexcept;
  static void writeLine() noexcept;
  static void writeErrorLine() noexcept;

  template <typename... Args>
  static void write(std::string_view format, Args... arguments) noexcept {
    init();
    try {
      std::cout << std::vformat(format, std::make_format_args(arguments...));
    } catch (...) {
    }
  }

  template <typename... Args>
  static void writeLine(std::string_view format, Args... arguments) {
    write(format, arguments...);
    cache_m &= ~(1 << 4);
    try {
      std::cout << std::endl;
    } catch (...) {
    }
  }

  template <typename... Args>
  static void writeError(std::string_view format, Args... arguments) {
    init();
    if (cache_m & 1 << 4) {
      cache_m &= ~(1 << 4);
      flushOutput();
    }
    try {
      std::cerr << std::vformat(format, std::make_format_args(arguments...));
    } catch (...) {
    }
  }

  template <typename... Args>
  static void writeErrorLine(std::string_view format, Args... arguments) {
    writeError(format, arguments...);
    try {
      std::cerr << std::endl;
    } catch (...) {
    }
  }
};
} // namespace tmk
