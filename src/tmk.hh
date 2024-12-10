#pragma once

#include <iostream>
#include <format>

namespace tmk {
class StreamRedirectionException : public std::exception {};

class Terminal {
private:
  static uint8_t cache_m;

#if defined(_WIN32)
  static void enableAnsiParse() noexcept;
#endif
  static void cacheStreamStates() noexcept;

public:
  static void init() noexcept;
  static bool isInputRedirected() noexcept;
  static bool isOutputRedirected() noexcept;
  static bool isErrorRedirected() noexcept;

  template <typename ...Args>
  static void write(std::string_view format, Args... arguments) {
    init();
    std::cout << std::vformat(format, std::make_format_args(arguments...));
  }

  template <typename ...Args>
  static void writeLine(std::string_view format, Args... arguments) {
    write(format, arguments...);
    std::cout << std::endl;
  }

  template <typename ...Args>
  static void writeError(std::string_view format, Args... arguments) {
    init();
    std::cerr << std::vformat(format, std::make_format_args(arguments...));
  }

  template <typename ...Args>
  static void writeErrorLine(std::string_view format, Args... arguments) {
    writeError(format, arguments...);
    std::cerr << std::endl;
  }
};
} // namespace tmk
