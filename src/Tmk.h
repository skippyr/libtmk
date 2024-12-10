#pragma once

#include <format>
#include <iostream>
#if defined(__linux__)
#include <cstdint>
#endif
#if !defined(__APPLE__)
#include <vector>
#endif

namespace tmk {
/** Contains the terminal layers where colors can be set into. */
enum class Layer {
    /** The foreground layer refers to the one where the graphemes are. */
    Foreground = 3,
    /** The background layer refers to the one behind the graphemes. */
    Background
};

/**
 * Contains the first 16 colors of the ANSI color palette, which map to colors
 * of the terminal colorscheme.
 */
enum class AnsiColor {
    /** The dark variant of the black color. */
    DarkBlack,
    /** The dark variant of the red color. */
    DarkRed,
    /** The dark variant of the green color. */
    DarkGreen,
    /** The dark variant of the yellow color. */
    DarkYellow,
    /** The dark variant of the blue color. */
    DarkBlue,
    /** The dark variant of the magenta color. */
    DarkMagenta,
    /** The dark variant of the cyan color. */
    DarkCyan,
    /** The dark variant of the white color. */
    DarkWhite,
    /** The light variant of the black color. */
    LightBlack,
    /** The light variant of the red color. */
    LightRed,
    /** The light variant of the green color. */
    LightGreen,
    /** The light variant of the yellow color. */
    LightYellow,
    /** The light variant of the blue color. */
    LightBlue,
    /** The light variant of the magenta color. */
    LightMagenta,
    /** The light variant of the cyan color. */
    LightCyan,
    /** The light variant of the white color. */
    LightWhite
};

/** Contains the available terminal font weights. */
enum class FontWeight {
    /** Usually rendered with bold weight and/or light colors. */
    Bold = 1,
    /** Rendered with faint colors. */
    Light
};

/** Contains the available terminal cursor shapes. */
enum class CursorShape {
    /** Fills the whole cursor cell. */
    Block = 2,
    /** Fills a portion of the bottom of the cursor cell. */
    Underline = 4,
    /** Fills a portion of the left side of the cursor cell. */
    Bar = 6
};

/** Contains the available directions the terminal cursor can move to. */
enum class Direction {
    /** The up direction. */
    Up = 'A',
    /** The down direction. */
    Down,
    /** The right direction. */
    Right,
    /** The left direction. */
    Left
};

/**
 * Represents an exception thrown when a set of standard terminal streams is
 * being redirected.
 */
class StreamRedirectionException : public std::exception {};

/**
 * Represents an exception thrown when a value is outside of its valid range.
 */
class OutOfBoundsException : public std::exception {};

/** Represents a color in RGB format. */
class RgbColor {
  private:
    /** The red component of the color. */
    uint8_t red;
    /** The green component of the color. */
    uint8_t green;
    /** The blue component of the color. */
    uint8_t blue;

  public:
    /**
     * Creates an instance of the RgbColor class with the given components.
     * @param red The red component of the color.
     * @param green The green component of the color.
     * @param blue The blue component of the color.
     */
    RgbColor(uint8_t red, uint8_t green, uint8_t blue) noexcept;
    /**
     * Gets the red component of the color.
     * @returns The red component of the color.
     */
    uint8_t getRed() const noexcept;
    /**
     * Sets the red component of the color.
     * @param red The red component to be set.
     */
    void setRed(uint8_t red) noexcept;
    /**
     * Gets the green component of the color.
     * @returns The green component of the color.
     */
    uint8_t getGreen() const noexcept;
    /**
     * Sets the green component of the color.
     * @param red The green component to be set.
     */
    void setGreen(uint8_t green) noexcept;
    /**
     * Gets the blue component of the color.
     * @returns The blue component of the color.
     */
    uint8_t getBlue() const noexcept;
    /**
     * Sets the blue component of the color.
     * @param red The blue component to be set.
     */
    void setBlue(uint8_t blue) noexcept;
};

/** Represents a coordinate within the terminal window cell grid. */
class Coordinate {
  private:
    /** The column component of the coordinate. */
    uint16_t column;
    /** The row component of the coordinate. */
    uint16_t row;

  public:
    /**
     * Creates an instance of the Coordinate class with the given components.
     * @param column The column component of the coordinate.
     * @param row The row component of the coordinate.
     */
    Coordinate(uint16_t column, uint16_t row) noexcept;
    /**
     * Gets the column component of the coordinate.
     * @returns The column component of the coordinate.
     */
    uint16_t getColumn() const noexcept;
    /**
     * Sets the column component of the coordinate.
     * @param column The column component to be set.
     */
    void setColumn(uint16_t column) noexcept;
    /**
     * Gets the row component of the coordinate.
     * @returns The row component of the coordinate.
     */
    uint16_t getRow() const noexcept;
    /**
     * Sets the row component of the coordinate.
     * @param row The row component to be set.
     */
    void setRow(uint16_t row) noexcept;
};

/** Represents dimensions within the terminal window. */
class Dimensions {
  private:
    /** The total columns in the dimensions. */
    uint16_t totalColumns;
    /** The total rows in the dimensions. */
    uint16_t totalRows;

  public:
    /**
     * Creates an instance of the Dimensions class with the given values.
     * @param totalColumns The total columns in the dimensions.
     * @param totalRows The total rows in the dimensions.
     */
    Dimensions(uint16_t totalColumns, uint16_t totalRows) noexcept;
    /**
     * Gets the total columns in the dimensions.
     * @returns The total columns in the dimensions.
     */
    uint16_t getTotalColumns() const noexcept;
    /**
     * Gets the total rows in the dimensions.
     * @returns The total rows in the dimensions.
     */
    uint16_t getTotalRows() const noexcept;
    /**
     * Gets the area of the dimensions.
     * @returns The area of the dimensions.
     */
    uint32_t getArea() const noexcept;
};

#if defined(_WIN32)
class Encoding {
  public:
    Encoding() = delete;
    static std::string convertUtf16ToUtf8(const std::wstring &utf16String);
    static std::wstring convertUtf8ToUtf16(const std::string &utf8String);
};
#endif

class MultiEncodingString {
  private:
    std::string m_utf8String;
#if defined(_WIN32)
    std::wstring m_utf16String;
#endif

  public:
    MultiEncodingString(const std::string &utf8String);
#if defined(_WIN32)
    MultiEncodingString(const std::wstring &utf16String);
#endif
    const std::string &AsUtf8String() const;
#if defined(_WIN32)
    const std::wstring &AsUtf16String() const;
#endif
};

class Terminal {
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
        if (!IsOutputRedirected()) {
            Write(format, arguments...);
            m_cache |= 1 << 4;
        } else if (!IsErrorRedirected()) {
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
    static void SetFontColor(const RgbColor &color, Layer layer) noexcept;
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
    static std::vector<MultiEncodingString>
    GetArguments(int totalMainArguments, const char **mainArguments);
    static void WriteLine() noexcept;
    static void WriteErrorLine() noexcept;

    template <typename... Args>
    static void Write(std::string_view format, Args... arguments) noexcept
    {
        Initialize();
        try {
            std::cout << std::vformat(format,
                                      std::make_format_args(arguments...));
        } catch (...) {
        }
    }

    template <typename... Args>
    static void WriteLine(std::string_view format, Args... arguments)
    {
        Write(format, arguments...);
        m_cache &= ~(1 << 4);
        try {
            std::cout << std::endl;
        } catch (...) {
        }
    }

    template <typename... Args>
    static void WriteError(std::string_view format, Args... arguments)
    {
        Initialize();
        if (m_cache & 1 << 4) {
            m_cache &= ~(1 << 4);
            FlushOutput();
        }
        try {
            std::cerr << std::vformat(format,
                                      std::make_format_args(arguments...));
        } catch (...) {
        }
    }

    template <typename... Args>
    static void WriteErrorLine(std::string_view format, Args... arguments)
    {
        WriteError(format, arguments...);
        try {
            std::cerr << std::endl;
        } catch (...) {
        }
    }
};
}
