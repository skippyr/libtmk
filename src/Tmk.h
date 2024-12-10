#pragma once

#include <format>
#include <iostream>
#if defined(__linux__)
#include <cstdint>
#endif
#if !defined(__APPLE__)
#include <vector>
#endif

namespace Tmk
{
    enum class Layer
    {
        Foreground = 3,
        Background
    };

    enum class AnsiColor
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

    enum class FontWeight
    {
        Bold = 1,
        Light
    };

    enum class CursorShape
    {
        Block = 2,
        Underline = 4,
        Bar = 6
    };

    enum class Direction
    {
        Up = 'A',
        Down,
        Right,
        Left
    };

    class StreamRedirectionException : public std::exception
    {
    };
    class OutOfBoundsException : public std::exception
    {
    };

    class RgbColor
    {
    private:
        uint8_t m_red;
        uint8_t m_green;
        uint8_t m_blue;

    public:
        RgbColor(uint8_t red, uint8_t green, uint8_t blue) noexcept;
        uint8_t GetRed() const noexcept;
        void SetRed(uint8_t red) noexcept;
        uint8_t GetGreen() const noexcept;
        void SetGreen(uint8_t green) noexcept;
        uint8_t GetBlue() const noexcept;
        void SetBlue(uint8_t blue) noexcept;
    };

    class Coordinate
    {
    private:
        uint16_t m_column;
        uint16_t m_row;

    public:
        Coordinate(uint16_t column, uint16_t row) noexcept;
        uint16_t GetColumn() const noexcept;
        void SetColumn(uint16_t column) noexcept;
        uint16_t GetRow() const noexcept;
        void SetRow(uint16_t row) noexcept;
    };

    class Dimensions
    {
    private:
        uint16_t m_totalColumns;
        uint16_t m_totalRows;

    public:
        Dimensions(uint16_t totalColumns, uint16_t totalRows) noexcept;
        uint16_t GetTotalColumns() const noexcept;
        uint16_t GetTotalRows() const noexcept;
        uint32_t GetArea() const noexcept;
    };

#if defined(_WIN32)
    class Encoding
    {
    public:
        Encoding() = delete;
        static std::string ConvertUtf16ToUtf8(const std::wstring& utf16String);
        static std::wstring ConvertUtf8ToUtf16(const std::string& utf8String);
    };
#endif

    class MultiEncodingString
    {
    private:
        std::string m_utf8String;
#if defined(_WIN32)
        std::wstring m_utf16String;
#endif

    public:
        MultiEncodingString(const std::string& utf8String);
#if defined(_WIN32)
        MultiEncodingString(const std::wstring& utf16String);
#endif
        const std::string& AsUtf8String() const;
#if defined(_WIN32)
        const std::wstring& AsUtf16String() const;
#endif
    };

    class Terminal
    {
    private:
        static uint8_t m_cache;

#if defined(_WIN32)
        static void EnableAnsiParse() noexcept;
#else
        static void SetRawInput(bool isRaw) noexcept;
        static void SetBlockingInput(bool isBlocking) noexcept;
#endif
        static void CacheStreamStates() noexcept;

        template <typename... Args>
        static void WriteAnsi(std::string_view format, Args... arguments)
        {
            if (!IsOutputRedirected())
            {
                Write(format, arguments...);
                m_cache |= 1 << 4;
            }
            else if (!IsErrorRedirected())
            {
                WriteError(format, arguments...);
            }
        }

    public:
        Terminal() = delete;
        static void Initialize() noexcept;
        static void FlushOutput() noexcept;
        static void ClearInput() noexcept;
        static bool IsInputRedirected() noexcept;
        static bool IsOutputRedirected() noexcept;
        static bool IsErrorRedirected() noexcept;
        static void SetFontColor(AnsiColor color, Layer layer) noexcept;
        static void SetFontColor(const RgbColor& color, Layer layer) noexcept;
        static void ResetFontColors() noexcept;
        static void SetFontWeight(FontWeight weight) noexcept;
        static void ResetFontWeight() noexcept;
        static void SetCursorVisible(bool isVisible) noexcept;
        static void SetCursorShape(CursorShape shape, bool shouldBlink) noexcept;
        static void ResetCursorShape() noexcept;
        static Coordinate GetCursorCoordinate();
        static void SetCursorCoordinate(Coordinate coordinate);
        static Coordinate MoveCursor(uint16_t steps, Direction direction);
        static Dimensions GetWindowDimensions();
        static void ClearWindow() noexcept;
        static void ClearLine() noexcept;
        static void RingBell() noexcept;
        static void OpenAlternateWindow() noexcept;
        static void CloseAlternateWindow() noexcept;
        static std::vector<MultiEncodingString> GetArguments(int totalMainArguments, const char** mainArguments);
        static void WriteLine() noexcept;
        static void WriteErrorLine() noexcept;

        template <typename... Args>
        static void Write(std::string_view format, Args... arguments) noexcept
        {
            Initialize();
            try
            {
                std::cout << std::vformat(format, std::make_format_args(arguments...));
            }
            catch (...)
            {
            }
        }

        template <typename... Args>
        static void WriteLine(std::string_view format, Args... arguments)
        {
            Write(format, arguments...);
            m_cache &= ~(1 << 4);
            try
            {
                std::cout << std::endl;
            }
            catch (...)
            {
            }
        }

        template <typename... Args>
        static void WriteError(std::string_view format, Args... arguments)
        {
            Initialize();
            if (m_cache & 1 << 4)
            {
                m_cache &= ~(1 << 4);
                FlushOutput();
            }
            try
            {
                std::cerr << std::vformat(format, std::make_format_args(arguments...));
            }
            catch (...)
            {
            }
        }

        template <typename... Args>
        static void WriteErrorLine(std::string_view format, Args... arguments)
        {
            WriteError(format, arguments...);
            try
            {
                std::cerr << std::endl;
            }
            catch (...)
            {
            }
        }
    };
}
