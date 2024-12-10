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
    /// <summary>
    /// Contains the terminal layers where colors can be set into.
    /// </summary>
    enum class Layer
    {
        /// <summary>
        /// The foreground layer refers to the one where the graphemes are.
        /// </summary>
        Foreground = 3,
        /// <summary>
        /// The background layer refers to the one behind the graphemes.
        /// </summary>
        Background
    };

    /// <summary>
    /// Contains the first 16 colors of the ANSI color palette, which map to the colors of the terminal colorscheme.
    /// </summary>
    enum class AnsiColor
    {
        /// <summary>
        /// The dark variant of the black color.
        /// </summary>
        DarkBlack,
        /// <summary>
        /// The dark variant of the red color.
        /// </summary>
        DarkRed,
        /// <summary>
        /// The dark variant of the green color.
        /// </summary>
        DarkGreen,
        /// <summary>
        /// The dark variant of the yellow color.
        /// </summary>
        DarkYellow,
        /// <summary>
        /// The dark variant of the blue color.
        /// </summary>
        DarkBlue,
        /// <summary>
        /// The dark variant of the magenta color.
        /// </summary>
        DarkMagenta,
        /// <summary>
        /// The dark variant of the cyan color.
        /// </summary>
        DarkCyan,
        /// <summary>
        /// The dark variant of the white color.
        /// </summary>
        DarkWhite,
        /// <summary>
        /// The light variant of the black color.
        /// </summary>
        LightBlack,
        /// <summary>
        /// The light variant of the red color.
        /// </summary>
        LightRed,
        /// <summary>
        /// The light variant of the green color.
        /// </summary>
        LightGreen,
        /// <summary>
        /// The light variant of the yellow color.
        /// </summary>
        LightYellow,
        /// <summary>
        /// The light variant of the blue color.
        /// </summary>
        LightBlue,
        /// <summary>
        /// The light variant of the magenta color.
        /// </summary>
        LightMagenta,
        /// <summary>
        /// The light variant of the cyan color.
        /// </summary>
        LightCyan,
        /// <summary>
        /// The light variant of the white color.
        /// </summary>
        LightWhite
    };

    /// <summary>
    /// Contains the available terminal font weights.
    /// </summary>
    enum class FontWeight
    {
        /// <summary>
        /// Usually rendered with bold weight and/or light colors.
        /// </summary>
        Bold = 1,
        /// <summary>
        /// Rendered with faint colors.
        /// </summary>
        Light
    };

    /// <summary>
    /// Contains the available terminal cursor shapes.
    /// </summary>
    enum class CursorShape
    {
        /// <summary>
        /// Fills the whole cursor cell.
        /// </summary>
        Block = 2,
        /// <summary>
        /// Fills a portion of the bottom of the cursor cell.
        /// </summary>
        Underline = 4,
        /// <summary>
        /// Fills a portion of the left side of the cursor cell.
        /// </summary>
        Bar = 6
    };

    /// <summary>
    /// Contains the available directions the terminal cursor can move to.
    /// </summary>
    enum class Direction
    {
        /// <summary>
        /// The up direction.
        /// </summary>
        Up = 'A',
        /// <summary>
        /// The down direction.
        /// </summary>
        Down,
        /// <summary>
        /// The right direction.
        /// </summary>
        Right,
        /// <summary>
        /// The left direction.
        /// </summary>
        Left
    };

    /// <summary>
    /// Represents an exception thrown when a set of standard terminal streams are being redirected.
    /// </summary>
    class StreamRedirectionException : public std::exception
    {
    };

    /// <summary>
    /// Represents an exception thrown when a value is outside of its valid range.
    /// </summary>
    class OutOfBoundsException : public std::exception
    {
    };

    /// <summary>
    /// Represents a color in RGB format.
    /// </summary>
    class RgbColor
    {
    private:
        /// <summary>
        /// The red component of the color.
        /// </summary>
        uint8_t m_red;
        /// <summary>
        /// The green component of the color.
        /// </summary>
        uint8_t m_green;
        /// <summary>
        /// The blue component of the color.
        /// </summary>
        uint8_t m_blue;

    public:
        /// <summary>
        /// Creates an instance of the RGB color class with the given components.
        /// </summary>
        /// <param name="red">The red component of the color.</param>
        /// <param name="green">The green component of the color.</param>
        /// <param name="blue">The blue component of the color.</param>
        RgbColor(uint8_t red, uint8_t green, uint8_t blue) noexcept;
        /// <summary>
        /// Gets the red component of the color.
        /// </summary>
        /// <returns>The red component of the color.</returns>
        uint8_t GetRed() const noexcept;
        /// <summary>
        /// Sets the red component of the color.
        /// </summary>
        /// <param name="red">The red component to be set.</param>
        void SetRed(uint8_t red) noexcept;
        /// <summary>
        /// Gets the green component of the color.
        /// </summary>
        /// <returns>The green component of the color.</returns>
        uint8_t GetGreen() const noexcept;
        /// <summary>
        /// Sets the green component of the color.
        /// </summary>
        /// <param name="green">The green component to be set.</param>
        void SetGreen(uint8_t green) noexcept;
        /// <summary>
        /// Gets the blue component of the color.
        /// </summary>
        /// <returns>The blue component of the color.</returns>
        uint8_t GetBlue() const noexcept;
        /// <summary>
        /// Sets the blue component of the color.
        /// </summary>
        /// <param name="blue">The blue component to be set.</param>
        void SetBlue(uint8_t blue) noexcept;
    };


    /// <summary>
    /// Represents a coordinate within the terminal window cell grid.
    /// </summary>
    class Coordinate
    {
    private:
        /// <summary>
        /// The column component of the coordinate.
        /// </summary>
        uint16_t m_column;
        /// <summary>
        /// The row component of the coordinate.
        /// </summary>
        uint16_t m_row;

    public:
        /// <summary>
        /// Creates an instance of the Coordinate class with the given components.
        /// </summary>
        /// <param name="column">The column component of the coordinate.</param>
        /// <param name="row">The row component of the coordinate.</param>
        Coordinate(uint16_t column, uint16_t row) noexcept;
        /// <summary>
        /// Gets the column component of the coordinate.
        /// </summary>
        /// <returns>The column component of the coordinate.</returns>
        uint16_t GetColumn() const noexcept;
        /// <summary>
        /// Sets the column component of the coordinate.
        /// </summary>
        /// <param name="column">The column component to be set.</param>
        void SetColumn(uint16_t column) noexcept;
        /// <summary>
        /// Gets the row component of the coordinate.
        /// </summary>
        /// <returns>The row component of the coordinate.</returns>
        uint16_t GetRow() const noexcept;
        /// <summary>
        /// Sets the row component of the coordinate.
        /// </summary>
        /// <param name="row">The row component to be set.</param>
        void SetRow(uint16_t row) noexcept;
    };

    /// <summary>
    /// Represents dimensions within the terminal window.
    /// </summary>
    class Dimensions
    {
    private:
        /// <summary>
        /// The total columns in the dimensions.
        /// </summary>
        uint16_t m_totalColumns;
        /// <summary>
        /// The total rows in the dimensions.
        /// </summary>
        uint16_t m_totalRows;

    public:
        /// <summary>
        /// Creates an instance of the Dimensions class with the given values.
        /// </summary>
        /// <param name="totalColumns">The total columns in the dimensions.</param>
        /// <param name="totalRows">The total rows in the dimensions.</param>
        Dimensions(uint16_t totalColumns, uint16_t totalRows) noexcept;
        /// <summary>
        /// Gets the total columns in the dimensions.
        /// </summary>
        /// <returns>The total columns in the dimensions.</returns>
        uint16_t GetTotalColumns() const noexcept;
        /// <summary>
        /// Gets the total rows in the dimensions.
        /// </summary>
        /// <returns>The total rows in the dimensions.</returns>
        uint16_t GetTotalRows() const noexcept;
        /// <summary>
        /// Gets the area of the dimensions.
        /// </summary>
        /// <returns>The area of the dimensions.</returns>
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
