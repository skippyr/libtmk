#pragma once

#include <iostream>

namespace tmk {
  /** @brief Represents the terminal. */
  class Terminal final {
  private:
    static bool isInputRedirected_m;
    static bool isOutputRedirected_m;
    static bool isErrorRedirected_m;
    static bool hasSetup_m;

#if defined(_WIN32)
    static void setUtf8Encoding();
    static void enableAnsiParse();
#endif
    static void setup();

  public:
    Terminal() = delete;
    /**
     * Checks if the input stream is redirected.
     * @returns A boolean that states the input stream is redirected.
     */
    static bool isInputRedirected();
    /**
     * Checks if the output stream is redirected.
     * @returns A boolean that states the output stream is redirected.
     */
    static bool isOutputRedirected();
    /**
     * Checks if the error stream is redirected.
     * @returns A boolean that states the error stream is redirected.
     */
    static bool isErrorRedirected();

    /**
     * Formats and writes a string to the output stream.
     * @tparam Args A parameter pack containing the arguments to be formatted.
     * @param format The format to be used. It accepts the same specifiers as
     * the std::format function family.
     * @param arguments The arguments to be formatted.
     */
    template <class... Args>
    static void write(const std::string &format, Args... arguments) {
      setup();
      std::cout << std::vformat(format, std::make_format_args(arguments...));
    }

    /**
     * Formats and writes a string to the output stream with a newline append to
     * its end.
     * @tparam Args A parameter pack containing the arguments to be formatted.
     * @param format The format to be used. It accepts the same specifiers as
     * the std::format function family.
     * @param arguments The arguments to be formatted.
     */
    template <class... Args>
    static void writeLine(const std::string &format, Args... arguments) {
      write(format, arguments...);
      std::cout << std::endl;
    }

    /**
     * Formats and writes a string to the error stream.
     * @tparam Args A parameter pack containing the arguments to be formatted.
     * @param format The format to be used. It accepts the same specifiers as
     * the std::format function family.
     * @param arguments The arguments to be formatted.
     */
    template <class... Args>
    static void writeError(const std::string &format, Args... arguments) {
      setup();
      std::cerr << std::vformat(format, std::make_format_args(arguments...));
    }

    /**
     * Formats and writes a string to the error stream with a newline append to
     * its end.
     * @tparam Args A parameter pack containing the arguments to be formatted.
     * @param format The format to be used. It accepts the same specifiers as
     * the std::format function family.
     * @param arguments The arguments to be formatted.
     */
    template <class... Args>
    static void writeErrorLine(const std::string &format, Args... arguments) {
      writeError(format, arguments...);
      std::cerr << std::endl;
    }
  };
}
