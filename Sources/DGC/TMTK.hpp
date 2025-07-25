/* //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\
 * TMTK.hpp
 *
 * TMTK (Terminal Manipulation ToolKit)
 * DGC :: Sherman Rofeman <skippyr.developer@icloud.com>
 *
 * Distribuded under the BSD-3-Clause License.
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

    class InitException : public std::exception
    {
    };

#ifdef _WIN32
    class CannotSetOutputCPException final : public InitException
    {
    };

    class InvalidHandleValueException final : public InitException
    {
    };

    class InvalidFileTypeException final : public InitException
    {
    };

    class NoANSISupportException final : public InitException
    {
    };

    class FlushConsoleInputBufferException final : public std::exception
    {
    };

    class BadEncodingException final : public std::exception
    {
    };
#else
#ifdef __linux__
    class CannotOpenCommandLineException final : public std::exception
    {
    };

#endif
    class BadFileDescriptorException final : public InitException
    {
    };

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

    class Terminal final
    {
        static bool s_isInputRedirected;
        static bool s_isOutputRedirected;
        static bool s_isErrorRedirected;
        static bool s_allowsTextStyles;
#ifdef _WIN32
        static HANDLE s_inputHandle;
        static HANDLE s_outputHandle;
        static HANDLE s_errorHandle;
#endif
        static bool s_hasInit;
        static bool s_hasANSICache;
        static bool s_ansiPrefersStdOut;

        static void Init();
#ifdef _WIN32
        [[nodiscard]]
        static HANDLE GetHandle(DWORD id);
        [[nodiscard]]
        static bool IsStreamRedirected(HANDLE handle);
        [[nodiscard]]
        static bool EnableANSIParse(HANDLE handle);
        [[nodiscard]]
        static CONSOLE_SCREEN_BUFFER_INFO GetScreenBufferInfo();
#else
        [[nodiscard]]
        static bool IsStreamRedirected(int fd);
#endif

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
        static std::vector<UnicodeString> GetArguments();
        [[nodiscard]]
        static bool IsInputRedirected();
        [[nodiscard]]
        static bool IsOutputRedirected();
        [[nodiscard]]
        static bool IsErrorRedirected();
        static void FlushOutput();
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

        template <typename... Arguments>
        static void Write(const std::string_view& format, Arguments... arguments)
        {
            Init();
            std::string result = std::vformat(format, std::make_format_args(arguments...));
            std::cout << result;
            s_ansiPrefersStdOut = true;
            if (s_hasANSICache && result.contains('\n'))
            {
                s_hasANSICache = false;
            }
        }

        template <typename T>
        static void Write(const T& argument)
        {
            Write("{}", argument);
        }

        template <typename... Arguments>
        static void WriteLine(const std::string_view& format, Arguments... arguments)
        {
            Write(format, arguments...);
            Write("\n");
        }

        template <typename T>
        static void WriteLine(const T& argument)
        {
            WriteLine("{}", argument);
        }

        template <typename... Arguments>
        static void WriteError(const std::string_view& format, Arguments... arguments)
        {
            Init();
            if (s_hasANSICache)
            {
                FlushOutput();
            }
            std::cerr << std::vformat(format, std::make_format_args(arguments...));
            s_ansiPrefersStdOut = false;
        }

        template <typename T>
        static void WriteError(const T& argument)
        {
            WriteError("{}", argument);
        }

        template <typename... Arguments>
        static void WriteErrorLine(const std::string_view& format, Arguments... arguments)
        {
            WriteError(format, arguments...);
            WriteError("\n");
        }

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
