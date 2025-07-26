/* //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\
 * TMTK.hh
 *
 * TMTK (Terminal Manipulation ToolKit) <https://github.com/skippyr/TMTK>
 * DGC :: Sherman Rofeman (skippyr) <skippyr.developer@icloud.com>
 *
 * Distributed under the BSD-3-Clause License.
 * See the LICENSE file that comes with the source code for license and copyright details.
 * //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\
 */

#pragma once
#include <cstdint>
#include <format>
#include <iostream>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#endif

namespace DGC::TMTK
{
    enum class ANSIColor
    {
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

    enum class TextStyle
    {
        Bold = 1,
        Dim = 1 << 1,
        Italic = 1 << 2,
        Underline = 1 << 3,
        Strikethrough = 1 << 4,
        Blinking = 1 << 5,
        InvertedColors = 1 << 6,
        Hidden = 1 << 7
    };

    enum class CursorStyle
    {
        BlinkingBlock = 1,
        StaticBlock,
        BlinkingUnderline,
        StaticUnderline,
        BlinkingBar,
        StaticBar
    };

    enum class Direction
    {
        Top = 'A',
        Down,
        Right,
        Left
    };

    class Exception : public std::exception
    {
        std::string m_message;

    public:
        Exception(const std::string_view& message);
        [[nodiscard]]
        const char* what() const noexcept override;
    };

    class InitException : public Exception
    {
    public:
        InitException(const std::string_view& message);
    };

#ifdef _WIN32
    class BadEncodingException final : public std::exception
    {
    };
#else
#ifdef __linux__
    class CannotOpenCommandLineException final : public std::exception
    {
    };

#endif
    class TcgetattrException final : public std::exception
    {
    };

    class TcsetattrException final : public std::exception
    {
    };

    class FcntlException final : public std::exception
    {
    };

    class CursorFormatException final : public std::exception
    {
    };
#endif

    class StreamRedirectionException final : public std::exception
    {
    };

    class NotEnoughMemoryException final : public std::exception
    {
    };

    class OutOfRangeException final : public std::exception
    {
    };

    class CannotWriteToStreamException final : public std::exception
    {
    };

    class CannotFlushStreamException final : public std::exception
    {
    };

    class RGBColor final
    {
        std::uint8_t m_red;
        std::uint8_t m_green;
        std::uint8_t m_blue;

    public:
        constexpr RGBColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue) noexcept : m_red(red), m_green(green), m_blue(blue)
        {
        }

        [[nodiscard]]
        constexpr std::uint8_t GetRed() const noexcept
        {
            return m_red;
        }

        [[nodiscard]]
        constexpr std::uint8_t GetGreen() const noexcept
        {
            return m_green;
        }

        [[nodiscard]]
        constexpr std::uint8_t GetBlue() const noexcept
        {
            return m_blue;
        }

        constexpr void SetRed(std::uint8_t red) noexcept
        {
            m_red = red;
        }

        constexpr void SetGreen(std::uint8_t green) noexcept
        {
            m_green = green;
        }

        constexpr void SetBlue(std::uint8_t blue) noexcept
        {
            m_blue = blue;
        }
    };

    class Coordinate final
    {
        std::uint16_t m_column;
        std::uint16_t m_row;

    public:
        constexpr Coordinate(std::uint16_t column, std::uint16_t row) noexcept : m_column(column), m_row(row)
        {
        }

        [[nodiscard]]
        constexpr std::uint16_t GetColumn() const noexcept
        {
            return m_column;
        }

        [[nodiscard]]
        constexpr std::uint16_t GetRow() const noexcept
        {
            return m_row;
        }

        void SetColumn(std::uint16_t column) noexcept
        {
            m_column = column;
        }

        void SetRow(std::uint16_t row) noexcept
        {
            m_row = row;
        }
    };

    class Dimensions final
    {
        std::uint16_t m_width;
        std::uint16_t m_height;

    public:
        constexpr Dimensions(std::uint16_t width, std::uint16_t height) noexcept : m_width(width), m_height(height)
        {
        }

        [[nodiscard]]
        constexpr std::uint16_t GetWidth() const noexcept
        {
            return m_width;
        }

        [[nodiscard]]
        constexpr std::uint16_t GetHeight() const noexcept
        {
            return m_height;
        }

        constexpr void SetWidth(std::uint16_t width) noexcept
        {
            m_width = width;
        }

        constexpr void SetHeight(std::uint16_t height) noexcept
        {
            m_height = height;
        }

        [[nodiscard]]
        constexpr std::uint32_t GetArea() const noexcept
        {
            return m_width * m_height;
        }
    };

#ifdef _WIN32
    class Encoding final
    {
    public:
        Encoding() = delete;
        static std::wstring ConvertUTF8To16(const std::string_view& utf8String);
        static std::string ConvertUTF16To8(const std::wstring_view& utf16String);
    };
#endif

    class UnicodeString final
    {
#ifdef _WIN32
        std::wstring m_encodedInUTF16;
#endif
        std::string m_encodedInUTF8;

    public:
#ifdef _WIN32
        UnicodeString(const std::wstring_view& encodedInUTF16);
#else
        UnicodeString(const std::string_view& encodedInUTF8);
#endif
#ifdef _WIN32
        [[nodiscard]]
        const std::wstring& EncodedInUTF16() const;
#endif
        [[nodiscard]]
        const std::string& EncodedInUTF8() const;
    };

    /// <summary>
    /// Represents the terminal emulator.
    /// </summary>
    class Terminal final
    {
        /// <summary>
        /// A boolean that states the terminal input stream is being redirected.
        /// </summary>
        static bool s_isInputRedirected;
        /// <summary>
        /// A boolean that states the terminal output stream is being redirected.
        /// </summary>
        static bool s_isOutputRedirected;
        /// <summary>
        /// A boolean that states the terminal error stream is being redirected.
        /// </summary>
        static bool s_isErrorRedirected;
        /// <summary>
        /// A boolean that states terminal styles are enabled. It includes colors and text styles.
        /// </summary>
        static bool s_allowsStyles;
#ifdef _WIN32
        /// <summary>
        /// The Windows handle for the terminal input stream.
        /// </summary>
        static HANDLE s_inputHandle;
        /// <summary>
        /// The Windows handle for the terminal output stream.
        /// </summary>
        static HANDLE s_outputHandle;
        /// <summary>
        /// The Windows handle for the terminal error stream.
        /// </summary>
        static HANDLE s_errorHandle;
#endif
        /// <summary>
        /// A boolean that states the terminal has been initialized, setting up the required features in and from the environment.
        /// </summary>
        static bool s_hasInit;
        /// <summary>
        /// A boolean that states at least one ANSI sequence has been cached in the terminal output stream buffer.
        /// </summary>
        static bool s_hasANSICache;
        /// <summary>
        /// A boolean that states ANSI sequences are prefered to be written to the terminal output stream, allowing caching.
        /// </summary>
        static bool s_ansiPrefersStdOut;

        /// <summary>
        /// Initiates the required terminal features in and from the environment. It includes setting up UTF-8 as the output encoding, enable the parse of ANSI escape sequences,
        /// and cache metadata about streams and style allowance.
        /// </summary>
        /// <exception cref="CannotSetOutputEncodingException">Thrown, on Windows, when UTF-8 cannot be set as the output encoding.</exception>
        /// <exception cref="NoANSISupportException">Thrown, on Windows, when the parse of ANSI escape sequences cannot be enabled.</exception>
        /// <exception cref="InvalidHandleValueException">Thrown, on Windows, when any stream handle is invalid.</exception>
        /// <exception cref="InvalidFileTypeException">Thrown, on Windows, when any stream file type is invalid.</exception>
        /// <exception cref="InvalidFileDescriptorException">Thrown, on macOS and Linux, when the file descriptor is invalid.</exception>
        static void Init();
#ifdef _WIN32
        /// <summary>
        /// Gets the Windows terminal handle of a stream. It is only available on Windows.
        /// </summary>
        /// <param name="id">The stream identifier, such as <code>STD_OUTPUT_HANDLE</code>.</param>
        /// <returns>The handle.</returns>
        /// <exception cref="InvalidHandleValueException">Thrown when the stream handle is invalid.</exception>
        [[nodiscard]]
        static HANDLE GetHandle(DWORD id, const char* name);
        /// <summary>
        /// Checks whether a terminal stream is being redirected. This overloading is only available on Windows.
        /// </summary>
        /// <param name="handle">The stream handle.</param>
        /// <returns>A boolean that states that.</returns>
        /// <exception cref="InvalidFileTypeException">Thrown when the stream file type is invalid.</exception>
        [[nodiscard]]
        static bool IsStreamRedirected(HANDLE handle, const char* name);
        /// <summary>
        /// Sets the bitmask flag that enables the parse of ANSI escape sequences in a terminal stream mode. It is only available on Windows.
        /// </summary>
        /// <param name="handle">The stream handle.</param>
        /// <returns>A boolean that states the parse has been enabled.</returns>
        /// <remarks>It is only necessary to set it on one output stream for it to work.</remarks>
        [[nodiscard]]
        static bool EnableANSIParse(HANDLE handle) noexcept;
        /// <summary>
        /// Gets the Windows terminal screen buffer information.
        /// </summary>
        /// <returns>The buffer information.</returns>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initialized.</exception>
        /// <exception cref="StreamRedirectionException">Thrown when all possible streams that could return an answer are being redirected.</exception>
        [[nodiscard]]
        static CONSOLE_SCREEN_BUFFER_INFO GetScreenBufferInfo();
#else
        /// <summary>
        /// Checks whether a terminal stream is being redirected. This overloading is only available on macOS and Linux.
        /// </summary>
        /// <param name="fd">The stream file descriptor, such as <code>STDOUT_FILENO</code>.</param>
        /// <returns>A boolean that states that.</returns>
        /// <exception cref="InvalidFileDescriptorException">Thrown when the file descriptor is invalid.</exception>
        [[nodiscard]]
        static bool IsStreamRedirected(int fd, const char* name);
#endif

        /// <summary>
        /// Formats and writes an ANSI escape sequence to a terminal output stream.
        /// </summary>
        /// <typeparam name="Arguments">A parameter pack containing the arguments to be formatted.</typeparam>
        /// <param name="format">The format to be used.</param>
        /// <param name="arguments">The arguments to be formatted.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initialized.</exception>
        /// <exception cref="StreamRedirectionException">Thrown when both output and error streams are being redirected.</exception>
        template <typename... Arguments>
        static void WriteAnsi(const std::string_view& format, Arguments... arguments)
        {
            Init();
            if (s_ansiPrefersStdOut)
            {
                if (!s_isOutputRedirected)
                {
                    Write(format, arguments...);
                    s_hasANSICache = true;
                }
                else if (!s_isErrorRedirected)
                {
                    WriteError(format, arguments...);
                }
                else
                {
                    throw StreamRedirectionException();
                }
            }
            else
            {
                if (!s_isErrorRedirected)
                {
                    WriteError(format, arguments...);
                }
                else if (!s_isOutputRedirected)
                {
                    Write(format, arguments...);
                    s_hasANSICache = true;
                }
                else
                {
                    throw StreamRedirectionException();
                }
            }
        }

    public:
        Terminal() = delete;
        /// <summary>
        /// Gets the arguments given by the terminal shell to the current process.
        /// </summary>
        /// <returns>A vector containing the arguments.</returns>
        /// <exception cref="NotEnoughMemoryException">Thrown when memory cannot be allocated for the arguments.</exception>
        /// <exception cref="BadEncodingException">Thrown, on Windows, if any argument is badly encoded in UTF-16.</exception>
        /// <exception cref="CannotOpenCommandLineException">Thrown, on Linux, if the <code>/proc/self/cmdline</code> file cannot be opened for reading.</exception>
        static std::vector<UnicodeString> GetArguments();
        /// <summary>
        /// Checks whether the terminal input stream is being redirected.
        /// </summary>
        /// <returns>A boolean that states that.</returns>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initialized.</exception>
        [[nodiscard]]
        static bool IsInputRedirected();
        /// <summary>
        /// Checks whether the terminal output stream is being redirected.
        /// </summary>
        /// <returns>A boolean that states that.</returns>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initialized.</exception>
        [[nodiscard]]
        static bool IsOutputRedirected();
        /// <summary>
        /// Checks whether the terminal error stream is being redirected.
        /// </summary>
        /// <returns>A boolean that states that.</returns>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initialized.</exception>
        [[nodiscard]]
        static bool IsErrorRedirected();
        /// <summary>
        /// Flushes the terminal output stream buffer, writing any content it has to the stream.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initialized.</exception>
        /// <exception cref="CannotFlushStreamException">Thrown when the stream cannot be flushed.</exception>
        static void FlushOutput();
        /// <summary>
        /// Flushes the terminal input buffer, losing any content it has.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initialized.</exception>
        /// <exception cref="CannotFlushStreamException">Thrown when the stream cannot be flushed.</exception>
        static void FlushInput();
        static void SetAllowsTextStyle(bool allowsTextStyle);
        static void SetForeground(std::uint8_t ansiColor);
        static void SetForeground(ANSIColor color);
        static void SetForeground(RGBColor color);
        static void SetBackground(std::uint8_t ansiColor);
        static void SetBackground(ANSIColor color);
        static void SetBackground(RGBColor color);
        static void SetTextStyles(int styles);
        static void SetTextStyles(TextStyle style);
        static void ResetColors();
        static void ResetTextStyles();
        static void OpenAlternateScreen();
        static void CloseAlternateScreen();
        static Dimensions GetDimensions();
        static Coordinate GetCursorCoordinate();
        static void SetCursorCoordinate(std::uint16_t column, std::uint16_t row);
        static void SetCursorCoordinate(const Coordinate& coordinate);
        static void MoveCursor(std::uint16_t steps, Direction direction);
        static void SetCursorVisible(bool isVisible);
        static void SetCursorStyle(CursorStyle style);
        static void ResetCursorStyle();
        static void RingBell();
        static void ClearScreen();
        static void ClearLine();
        static void ClearHistory();

        /// <summary>
        /// Formats and writes a string to the terminal output stream.
        /// </summary>
        /// <param name="format">The format to be used. It accepts the same specifiers as the <code>std::println</code> function family.</param>
        /// <param name="arguments">The arguments to be formatted.</param>
        /// <typeparam name="Arguments">A parameter pack containing the arguments to be formatted.</typeparam>
        /// <exception cref="std::out_of_memory">Thrown when memory cannot be allocated to format the string.</exception>
        /// <exception cref="std::format_error">Thrown when an incorrect formatting is providing.</exception>
        /// <exception cref="CannotWriteToStreamException">Thrown when it is not possible to write to the stream.</exception>
        template <typename... Arguments>
        static void Write(const std::string_view& format, Arguments... arguments)
        {
            Init();
            std::string result = std::vformat(format, std::make_format_args(arguments...));
            std::cout.clear();
            std::cout << result;
            if (std::cout.fail())
            {
                std::cout.clear();
                throw CannotWriteToStreamException();
            }
            s_ansiPrefersStdOut = true;
            if (s_hasANSICache && result.contains('\n'))
            {
                s_hasANSICache = false;
            }
        }

        /// <summary>
        /// Formats and writes an argument to the terminal output stream.
        /// </summary>
        /// <param name="argument">The argument to be formatted.</param>
        /// <exception cref="std::out_of_memory">Thrown when memory cannot be allocated to format the string.</exception>
        /// <exception cref="CannotWriteToStreamException">Thrown when it is not possible to write to the stream.</exception>
        template <typename T>
        static void Write(const T& argument)
        {
            Write("{}", argument);
        }

        /// <summary>
        /// Formats and writes a string to the terminal output stream with a newline sequence appended to its end.
        /// </summary>
        /// <param name="format">The format to be used. It accepts the same specifiers as the <code>std::println</code> function family.</param>
        /// <param name="arguments">The arguments to be formatted.</param>
        /// <typeparam name="Arguments">A parameter pack containing the arguments to be formatted.</typeparam>
        /// <exception cref="std::out_of_memory">Thrown when memory cannot be allocated to format the string.</exception>
        /// <exception cref="std::format_error">Thrown when an incorrect formatting is providing.</exception>
        /// <exception cref="CannotWriteToStreamException">Thrown when it is not possible to write to the stream.</exception>
        template <typename... Arguments>
        static void WriteLine(const std::string_view& format, Arguments... arguments)
        {
            Write(format, arguments...);
            Write("\n");
        }

        /// <summary>
        /// Formats and writes an argument to the terminal output stream with a newline sequence appended to its end.
        /// </summary>
        /// <param name="argument">The argument to be formatted.</param>
        /// <exception cref="std::out_of_memory">Thrown when memory cannot be allocated to format the string.</exception>
        /// <exception cref="CannotWriteToStreamException">Thrown when it is not possible to write to the stream.</exception>
        template <typename T>
        static void WriteLine(const T& argument)
        {
            WriteLine("{}", argument);
        }

        /// <summary>
        /// Formats and writes a string to the terminal error stream.
        /// </summary>
        /// <param name="format">The format to be used. It accepts the same specifiers as the <code>std::println</code> function family.</param>
        /// <param name="arguments">The arguments to be formatted.</param>
        /// <typeparam name="Arguments">A parameter pack containing the arguments to be formatted.</typeparam>
        /// <exception cref="std::out_of_memory">Thrown when memory cannot be allocated to format the string.</exception>
        /// <exception cref="std::format_error">Thrown when an incorrect formatting is providing.</exception>
        /// <exception cref="CannotWriteToStreamException">Thrown when it is not possible to write to the stream.</exception>
        template <typename... Arguments>
        static void WriteError(const std::string_view& format, Arguments... arguments)
        {
            Init();
            if (s_hasANSICache)
            {
                FlushOutput();
            }
            std::cerr.clear();
            std::cerr << std::vformat(format, std::make_format_args(arguments...));
            if (std::cerr.fail())
            {
                std::cerr.clear();
                throw CannotWriteToStreamException();
            }
            s_ansiPrefersStdOut = false;
        }

        /// <summary>
        /// Formats and writes an argument to the terminal error stream.
        /// </summary>
        /// <param name="argument">The argument to be formatted.</param>
        /// <exception cref="std::out_of_memory">Thrown when memory cannot be allocated to format the string.</exception>
        /// <exception cref="CannotWriteToStreamException">Thrown when it is not possible to write to the stream.</exception>
        template <typename T>
        static void WriteError(const T& argument)
        {
            WriteError("{}", argument);
        }

        /// <summary>
        /// Formats and writes a string to the terminal error stream with a newline sequence appended to its end.
        /// </summary>
        /// <param name="format">The format to be used. It accepts the same specifiers as the <code>std::println</code> function family.</param>
        /// <param name="arguments">The arguments to be formatted.</param>
        /// <typeparam name="Arguments">A parameter pack containing the arguments to be formatted.</typeparam>
        /// <exception cref="std::out_of_memory">Thrown when memory cannot be allocated to format the string.</exception>
        /// <exception cref="std::format_error">Thrown when an incorrect formatting is providing.</exception>
        /// <exception cref="CannotWriteToStreamException">Thrown when it is not possible to write to the stream.</exception>
        template <typename... Arguments>
        static void WriteErrorLine(const std::string_view& format, Arguments... arguments)
        {
            WriteError(format, arguments...);
            WriteError("\n");
        }

        /// <summary>
        /// Formats and writes an argument to the terminal error stream with a newline sequence appended to its end.
        /// </summary>
        /// <param name="argument">The argument to be formatted.</param>
        /// <exception cref="std::out_of_memory">Thrown when memory cannot be allocated to format the string.</exception>
        /// <exception cref="CannotWriteToStreamException">Thrown when it is not possible to write to the stream.</exception>
        template <typename T>
        static void WriteErrorLine(const T& argument)
        {
            WriteErrorLine("{}", argument);
        }
    };

    int operator|(TextStyle style0, TextStyle style1);
    int operator|(int styles, TextStyle style);
    int operator|(TextStyle style, int styles);
}
