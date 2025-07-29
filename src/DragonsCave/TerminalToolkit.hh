/* //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\
 * TerminalToolkit.cc
 *
 * Terminal Toolkit <https://github.com/skippyr/TerminalToolkit>
 * Dragon's Cave :: Sherman Rofeman (skippyr) <skippyr.developer@icloud.com>
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

namespace DragonsCave::TerminalToolkit
{
    /// <summary>
    /// Contains the first 16 standard colors of the terminal ANSI palette.
    /// </summary>
    /// <remarks>
    ///     <para>• They always map to the ones in the current color scheme, providing a consistent and widely compatible option.</para>
    ///     <para>• They may be set using the <c>Terminal::SetForeground</c> and <c>Terminal::SetBackground</c> functions.</para>
    /// </remarks>
    enum class ANSIColor
    {
        /// <summary>
        /// The dark variant of the black color. It is the same as value 0.
        /// </summary>
        /// <remarks>It should match the color scheme background color.</remarks>
        DarkBlack,
        /// <summary>
        /// The dark variant of the red color. It is the same as value 1.
        /// </summary>
        DarkRed,
        /// <summary>
        /// The dark variant of the green color. It is the same as value 2.
        /// </summary>
        DarkGreen,
        /// <summary>
        /// The dark variant of the yellow color. It is the same as value 3.
        /// </summary>
        DarkYellow,
        /// <summary>
        /// The dark variant of the blue color. It is the same as value 4.
        /// </summary>
        DarkBlue,
        /// <summary>
        /// The dark variant of the magenta color. It is the same as value 5.
        /// </summary.
        DarkMagenta,
        /// <summary>
        /// The dark variant of the cyan color. It is the same as value 6.
        /// </summary>
        DarkCyan,
        /// <summary>
        /// The dark variant of the white color. It is the same as value 7.
        /// </summary>
        DarkWhite,
        /// <summary>
        /// The light variant of the black color. It is the same as value 8.
        /// </summary>
        /// <remarks>It should match a darker version of the foreground color, useful for fadded highlights.</remarks>
        LightBlack,
        /// <summary>
        /// The light variant of the red color. It is the same as value 9.
        /// </summary>
        LightRed,
        /// <summary>
        /// The light variant of the green color. It is the same as value 10.
        /// </summary>
        LightGreen,
        /// <summary>
        /// The light variant of the yellow color. It is the same as value 11.
        /// </summary>
        LightYellow,
        /// <summary>
        /// The light variant of the blue color. it is the same as value 12.
        /// </summary>
        LightBlue,
        /// <summary>
        /// The light variant of the magenta color. It is the same as value 13.
        /// </summary>
        LightMagenta,
        /// <summary>
        /// The light variant of the cyan color. It is the same as value 14.
        /// </summary>
        LightCyan,
        /// <summary>
        /// The light variant of the white color. It is the same as value 15.
        /// </summary>
        /// <remarks>It should match the color scheme foreground color.</remarks>
        LightWhite
    };

    /// <summary>
    /// Contains the terminal text styles with the best compatibility.
    /// </summary>
    /// <remarks>They may be set using the <c>Terminal::SetTextStyles</c> function. Apply multiple ones by creating a bitmask using the <c>|</c> (bitwise OR) between them.</remarks>
    enum class TextStyle
    {
        /// <summary>
        /// Identified in the ANSI standard as the strong font brightness, in modern terminals, it usually applies the bold font weight and/or lighter foreground colors.
        /// </summary>
        /// <remarks>Even though some terminals may allow, the library does not set the <c>Bold</c> and <c>Faint</c> styles at the same time.</remarks>
        Bold = 1,
        /// <summary>
        /// Identified in the ANSI standard as the weak font brightness, it usually applies dimmed foreground colors.
        /// </summary>
        /// <remarks>Even though some terminals may allow, the library does not set the <c>Bold</c> and <c>Faint</c> styles at the same time because they technically refer to the same attribute.</remarks>
        Dim = 1 << 1,
        /// <summary>
        /// Makes the text slanted to the right.
        /// </summary>
        /// <remarks>
        ///     <para>• The terminal font needs to have italic variants for it to be visible unless faux styles are supported.</para>
        ///     <para>• Terminals that uses bitmap fonts may use different foreground and background colors instead of applying the expected effect.</para>
        /// </remarks>
        Italic = 1 << 2,
        /// <summary>
        /// Draws a straight horizontal line below the text.
        /// </summary>
        /// <remarks>
        ///     <para>• Terminals that uses bitmap fonts may use different foreground and background colors instead of applying the expected effect.</para>
        ///     <para>• It is not possible to set more than one underline style at the same time.</para>
        /// </remarks>
        Underline = 1 << 3,
        /// <summary>
        /// Draws a curly horizontal line below the text.
        /// </summary>
        /// <remarks>
        ///     <para>• It is not well-supported on Windows terminals, causing it to not be applied: avoid it or handle it on each platform for the best results.
        ///     <para>• It is not possible to set more than one underline style at the same time.</para>
        /// </remarks>
        DoubleUnderline = 1 << 4,
        /// <summary>
        /// Draws a curly horizontal line below the text.
        /// </summary>
        /// <remarks>
        ///     <para>• It is not well-supported on Windows terminals, causing it to not be applied: avoid it or handle it on each platform for the best results.
        ///     <para>• It is not possible to set more than one underline style at the same time.</para>
        /// </remarks>
        SquigglyLine = 1 << 5,
        /// <summary>
        /// Draws a horizontal line across the text.
        /// </summary>
        /// <remarks>Terminals that uses bitmap fonts may use different foreground and background colors instead of applying the expected effect.</remarks>
        Strikethrough = 1 << 6,
        /// <summary>
        /// Makes the text blink in a slow pace.
        /// </summary>
        /// <remarks>Some terminals may intentionally not support this feature due to the possible overhead it causes or make it cause the same behavior as the <c>RapidBlink</c> effect.</remarks>
        SlowBlink = 1 << 7,
        /// <summary>
        /// Makes the text blink in a rapid pace.
        /// </summary>
        /// <remarks>Some terminals may intentionally not support this feature due to the possible overhead it causes or make it cause the same behavior as the <c>SlowBlink</c> effect.</remarks>
        RapidBlink = 1 << 8,
        /// <summary>
        /// Inverts the colors applied to the foreground and background.
        /// </summary>
        SwappedLayers = 1 << 9,
        /// <summary>
        /// Makes the text dimmed or invisible.
        /// </summary>
        Hidden = 1 << 10
    };

    /// <summary>
    /// Contains the available terminal cursor styles.
    /// </summary>
    /// <remarks>They may be set using the <c>Terminal::SetCursorStyle</c> function.</remarks>
    enum class CursorStyle
    {
        /// <summary>
        /// The blinking variant of the block shape.
        /// </summary>
        BlinkingBlock = 1,
        /// <summary>
        /// The static variant of the block shape.
        /// </summary>
        StaticBlock,
        /// <summary>
        /// The blinking variant of the underline shape.
        /// </summary>
        BlinkingUnderline,
        /// <summary>
        /// The static variant of the underline shape.
        /// </summary>
        StaticUnderline,
        /// <summary>
        /// The blinking variant of the bar shape.
        /// </summary>
        BlinkingBar,
        /// <summary>
        /// The static variant of the bar shape.
        /// </summary>
        StaticBar
    };

    /// <summary>
    /// Contains the available directions for cursor movement inside the terminal.
    /// </summary>
    enum class Direction
    {
        /// <summary>
        /// Moves the cursor up.
        /// </summary>
        Top = 'A',
        /// <summary>
        /// Moves the cursor down.
        /// </summary>
        Down,
        /// <summary>
        /// Moves the cursor right.
        /// </summary>
        Right,
        /// <summary>
        /// Moves the cursor left.
        /// </summary>
        Left
    };

    /// <summary>
    /// Represents a generic exception used for others to be built upon.
    /// </summary>
    class Exception : public std::exception
    {
        /// <summary>
        /// The message that describes why the exception was thrown.
        /// </summary>
        std::string m_message;

    public:
        /// <summary>
        /// Constructs a new instance of the <c>Exception</c> class.
        /// </summary>
        /// <param name="message">The message that describes why it has been thrown.</param>
        explicit Exception(const std::string_view& message);
        /// <summary>
        /// Gets the message that describes why the exception was thrown.
        /// </summary>
        /// <returns>The message.</returns>
        [[nodiscard]]
        const char* what() const noexcept override;
    };

    /// <summary>
    /// Represents an exception thrown when the terminal cannot initiate its features.
    /// </summary>
    class InitException : public Exception
    {
    public:
        /// <summary>
        /// Creates an instance of the <c>InitException</c> class.
        /// </summary>
        /// <param name="message">The message that describes why it has been thrown.</param>
        explicit InitException(const std::string_view& message);
    };

    /// <summary>
    /// Represents an exception thrown when not enough memory can be allocate for certain operation.
    /// </summary>
    class NotEnoughMemoryException final : public Exception
    {
    public:
        /// <summary>
        /// Creates an instance of the <c>NotEnoughMemoryException</c> class.
        /// </summary>
        /// <param name="message">The message that describes why it has been thrown.</param>
        explicit NotEnoughMemoryException(const std::string_view& message);
    };

#ifdef _WIN32
    /// <summary>
    /// Represents an exception thrown when a string is bad encoded to perform a certain operation. It is only available on Windows.
    /// </summary>
    class BadEncodingException final : public Exception
    {
    public:
        /// <summary>
        /// Creates an instance of the <c>BadEncodingException</c> class.
        /// </summary>
        /// <param name="message">The message that describes why it has been thrown.</param>
        explicit BadEncodingException(const std::string_view& message);
    };
#endif

    /// <summary>
    /// Represents a generic exception thrown when a certain IO operation fails.
    /// </summary>
    class IOException : public Exception
    {
    public:
        /// <summary>
        /// Creates an instance of the <c>IOException</c> class.
        /// </summary>
        /// <param name="message">The message that describes why it has been thrown.</param>
        explicit IOException(const std::string_view& message);
    };

    /// <summary>
    /// Represents a generic exception throw for an internal attributes failure, like not being able to fetch or set them.
    /// </summary>
    class InternalAttributesException : public Exception
    {
    public:
        /// <summary>
        /// Creates an instance of the <c>InternalAttributesException</c> class.
        /// </summary>
        /// <param name="message">The message that describes why it has been thrown.</param>
        explicit InternalAttributesException(const std::string_view& message);
    };

    /// <summary>
    /// Represents a generic exception thrown when an expected format is badly formed.
    /// </summary>
    class FormatException : public Exception
    {
    public:
        /// <summary>
        /// Creates an instance of the <c>FormatException</c> class.
        /// </summary>
        /// <param name="message">The message that describes why it has been thrown.</param>
        explicit FormatException(const std::string_view& message);
    };

    /// <summary>
    /// Represents an exception thrown when a set of terminal streams required for an operation are being redirected.
    /// </summary>
    class StreamRedirectionException final : public IOException
    {
    public:
        /// <summary>
        /// Creates an instance of the <c>StreamRedirectionException</c> class.
        /// </summary>
        /// <param name="message">The message that describes why it has been thrown.</param>
        explicit StreamRedirectionException(const std::string_view& message);
    };

    /// <remarks>
    /// Represents an exception thrown when a value is outside its valid range.
    /// </remarks>
    class OutOfRangeException final : public Exception
    {
    public:
        /// <summary>
        /// Creates an instance of the <c>OutOfRangeException</c> class.
        /// </summary>
        /// <param name="message">The message that describes why it has been thrown.</param>
        explicit OutOfRangeException(const std::string_view& message);
    };

    /// <summary>
    /// Represents an RGB color.
    /// </summary>
    class RGBColor final
    {
        /// <summary>
        /// The red component of the color.
        /// </summary>
        std::uint8_t m_red;
        /// <summary>
        /// The green component of the color.
        /// </summary>
        std::uint8_t m_green;
        /// <summary>
        /// The blue component of the color.
        /// </summary>
        std::uint8_t m_blue;

    public:
        /// <summary>
        /// Creates an instance of the <c>RGBColor</c> class.
        /// </summary>
        /// <param name="red">The red component of the color.</param>
        /// <param name="green">The green component of the color.</param>
        /// <param name="blue">The blue component of the color.</param>
        constexpr RGBColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue) noexcept : m_red(red), m_green(green), m_blue(blue)
        {
        }

        /// <summary>
        /// Gets the red component of the color.
        /// </summary>
        /// <returns>The component.</returns>
        [[nodiscard]]
        constexpr std::uint8_t GetRed() const noexcept
        {
            return m_red;
        }

        /// <summary>
        /// Gets the green component of the color.
        /// </summary>
        /// <returns>The component.</returns>
        [[nodiscard]]
        constexpr std::uint8_t GetGreen() const noexcept
        {
            return m_green;
        }

        /// <summary>
        /// Gets the blue component of the color.
        /// </summary>
        /// <returns>The component.</returns>
        [[nodiscard]]
        constexpr std::uint8_t GetBlue() const noexcept
        {
            return m_blue;
        }

        /// <summary>
        /// Sets the red component of the color.
        /// </summary>
        /// <param name="red">The new component.</param>
        constexpr void SetRed(std::uint8_t red) noexcept
        {
            m_red = red;
        }

        /// <summary>
        /// Sets the green component of the color.
        /// </summary>
        /// <param name="green">The new component.</param>
        constexpr void SetGreen(std::uint8_t green) noexcept
        {
            m_green = green;
        }

        /// <summary>
        /// Sets the blue component of the color.
        /// </summary>
        /// <param name="blue">The new component.</param>
        constexpr void SetBlue(std::uint8_t blue) noexcept
        {
            m_blue = blue;
        }
    };

    /// <summary>
    /// Represents a coordinate within the terminal screen.
    /// </summary>
    /// <remarks>It considers a coordinate system where the origin point is in the top left corner of the terminal screen. From there, the column (in the horizontal axis) and row (in the vertical axis) values increase going right and down respectively.</remarks>
    class Coordinate final
    {
        /// <summary>
        /// The column component of the coordinate.
        /// </summary>
        std::uint16_t m_column;
        /// <summary>
        /// The row component of the coordinate.
        /// </summary>
        std::uint16_t m_row;

    public:
        /// <summary>
        /// Creates an instance of the <c>Coordinate</c> class.
        /// </summary>
        /// <param name="column">The column component of the coordinate.</param>
        /// <param name="row">The row component of the coordinate.</param>
        constexpr Coordinate(std::uint16_t column, std::uint16_t row) noexcept : m_column(column), m_row(row)
        {
        }

        /// <summary>
        /// Gets the column component of the coordinate.
        /// </summary>
        /// <returns>The component.</returns>
        [[nodiscard]]
        constexpr std::uint16_t GetColumn() const noexcept
        {
            return m_column;
        }

        /// <summary>
        /// Gets the row component of the coordinate.
        /// </summary>
        /// <returns>The component.</returns>
        [[nodiscard]]
        constexpr std::uint16_t GetRow() const noexcept
        {
            return m_row;
        }

        /// <summary>
        /// Sets the column component of the coordinate
        /// </summary>
        /// <param name="column">The new component to be set.</param>
        void SetColumn(std::uint16_t column) noexcept
        {
            m_column = column;
        }

        /// <summary>
        /// Sets the row component of the coordinate
        /// </summary>
        /// <param name="row">The new component to be set.</param>
        void SetRow(std::uint16_t row) noexcept
        {
            m_row = row;
        }
    };

    /// <summary>
    /// Represents the terminal screen dimensions.
    /// </summary>
    class Dimensions final
    {
        /// <summary>
        /// The width component of the dimensions.
        /// </summary>
        std::uint16_t m_width;
        /// <summary>
        /// The height component of the dimensions.
        /// </summary>
        std::uint16_t m_height;

    public:
        /// <summary>
        /// Creates a new instance of the <c>Dimensions</c> class.
        /// </summary>
        /// <param name="width">The width component of the dimensions.</param>
        /// <param name="height">The height component of the dimensions.</param>
        constexpr Dimensions(std::uint16_t width, std::uint16_t height) noexcept : m_width(width), m_height(height)
        {
        }

        /// <summary>
        /// Gets the width component of the dimensions.
        /// </summary>
        /// <returns>The component.</returns>
        [[nodiscard]]
        constexpr std::uint16_t GetWidth() const noexcept
        {
            return m_width;
        }

        /// <summary>
        /// Gets the height component of the dimensions.
        /// </summary>
        /// <returns>The component.</returns>
        [[nodiscard]]
        constexpr std::uint16_t GetHeight() const noexcept
        {
            return m_height;
        }

        /// <summary>
        /// Sets the width component of the dimensions.
        /// </summary>
        /// <returns>The new component to be set.</returns>
        constexpr void SetWidth(std::uint16_t width) noexcept
        {
            m_width = width;
        }

        /// <summary>
        /// Sets the height component of the dimensions.
        /// </summary>
        /// <returns>The new component to be set.</returns>
        constexpr void SetHeight(std::uint16_t height) noexcept
        {
            m_height = height;
        }

        /// <summary>
        /// Gets the area of the dimensions.
        /// </summary>
        /// <returns>The area.</returns>
        [[nodiscard]]
        constexpr std::uint32_t GetArea() const noexcept
        {
            return m_width * m_height;
        }
    };

#ifdef _WIN32
    /// <summary>
    /// Represents the terminal text encoding, allowing conversions. It is only available on Windows.
    /// </summary>
    class Encoding final
    {
    public:
        Encoding() = delete;
        /// <summary>
        /// Converts a UTF-8 encoded string to UTF-16.
        /// </summary>
        /// <param name="utf8String">The string to be converted. It must be null terminated.</param>
        /// <exception cref="OutOfRangeException">Thrown when the string provided is not null terminated.</exception>
        /// <exception cref="BadEncodingException">Thrown when the string provided is badly encoded.</exception>
        /// <exception cref="std::out_of_memory">Throw when not enough memory can be allocated for the converted string.</exception>
        static std::wstring FromUTF8To16(const std::string_view& utf8String);
        /// <summary>
        /// Converts a UTF-16 encoded string to UTF-8.
        /// </summary>
        /// <param name="utf16String">The string to be converted. It must be null terminated.</param>
        /// <exception cref="OutOfRangeException">Thrown when the string provided is not null terminated.</exception>
        /// <exception cref="BadEncodingException">Thrown when the string provided is badly encoded.</exception>
        /// <exception cref="std::out_of_memory">Throw when not enough memory can be allocated for the converted string.</exception>
        static std::string FromUTF16To8(const std::wstring_view& utf16String);
    };
#endif

    /// <summary>
    /// Represents a string encoded in different Unicode encodings.
    /// </summary>
    class UnicodeString final
    {
#ifdef _WIN32
        /// <summary>
        /// The string encoded in UTF-16.
        /// </summary>
        std::wstring m_encodedInUTF16;
#endif
        /// <summary>
        /// The string encoded in UTF-8.
        /// </summary>
        std::string m_encodedInUTF8;

    public:
#ifdef _WIN32
        /// <summary>
        /// Creates an instance of the <c>UnicodeString</c> class.
        /// </summary>
        /// <param name="encodedInUTF16">The base string encoded in UTF-16 to be copied from.</param>
        /// <exception cref="std::out_of_memory">Throw when not enough memory can be allocated for the copied string.</exception>
        UnicodeString(const std::wstring_view& encodedInUTF16);
#else
        /// <summary>
        /// Creates an instance of the <c>UnicodeString</c> class.
        /// </summary>
        /// <param name="encodedInUTF8">The base string encoded in UTF-8 to be copied from.</param>
        /// <exception cref="std::out_of_memory">Throw when not enough memory can be allocated for the copied string.</exception>
        UnicodeString(const std::string_view& encodedInUTF8);
#endif
#ifdef _WIN32
        /// <summary>
        /// Gets the string encoded in UTF-16.
        /// </summary>
        /// <returns>The string.</returns>
        [[nodiscard]]
        const std::wstring& EncodedInUTF16() const;
#endif
        /// <summary>
        /// Gets the string encoded in UTF-8.
        /// </summary>
        /// <returns>The string.</returns>
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
        /// A boolean that states terminal styles are allowed, including colors and text styles.
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
        /// A boolean that states the terminal features have been initiated.
        /// </summary>
        static bool s_hasInit;
        /// <summary>
        /// A boolean that states at least one ANSI sequence has been cached in the terminal output stream buffer.
        /// </summary>
        static bool s_hasANSICache;
        /// <summary>
        /// A boolean that states ANSI sequences are preferred to be written to the terminal output stream, allowing caching.
        /// </summary>
        static bool s_ansiPrefersStdOut;

        /// <summary>
        /// Initiates the required terminal features. It sets up UTF-8 as the output encoding, enables the parse of ANSI escape sequences, and caches metadata about streams and style allowance.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        static void Init();
#ifdef _WIN32
        /// <summary>
        /// Gets the Windows terminal handle of a stream. It is only available on Windows.
        /// </summary>
        /// <param name="id">The stream identifier, such as <c>STD_OUTPUT_HANDLE</c>.</param>
        /// <param name="name">The stream name to be used in exception messages.</param>
        /// <returns>The handle.</returns>
        /// <exception cref="InitException">Thrown when the stream handle is invalid.</exception>
        [[nodiscard]]
        static HANDLE GetHandle(DWORD id, const char* name);
        /// <summary>
        /// Checks whether a terminal stream is being redirected. It is only available on Windows.
        /// </summary>
        /// <param name="handle">The stream handle.</param>
        /// <param name="name">The stream name to be used in exception messages.</param>
        /// <returns>A boolean that states that.</returns>
        /// <exception cref="InitException">Thrown when the stream file type is invalid.</exception>
        [[nodiscard]]
        static bool IsStreamRedirected(HANDLE handle, const char* name);
        /// <summary>
        /// Sets the Windows bitmask flag that enables the parse of ANSI sequences in a terminal output stream mode. It is only available on Windows.
        /// </summary>
        /// <param name="handle">The stream handle.</param>
        /// <returns>A boolean that states the parse has been enabled.</returns>
        /// <remarks>It is only necessary to set it in one stream for it to work.</remarks>
        [[nodiscard]]
        static bool EnableANSIParse(HANDLE handle) noexcept;
        /// <summary>
        /// Gets the Windows terminal screen buffer information.
        /// </summary>
        /// <returns>The buffer information.</returns>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="StreamRedirectionException">Thrown when all possible streams that could return a response are being redirected.</exception>
        [[nodiscard]]
        static CONSOLE_SCREEN_BUFFER_INFO GetScreenBufferInfo();
#else
        /// <summary>
        /// Checks whether a terminal stream is being redirected. It is only available on macOS and Linux.
        /// </summary>
        /// <param name="fd">The stream file descriptor, such as <c>STDOUT_FILENO</c>.</param>
        /// <returns>A boolean that states that.</returns>
        /// <exception cref="InitException">Thrown when the file descriptor is invalid.</exception>
        [[nodiscard]]
        static bool IsStreamRedirected(int fd, const char* name);
#endif

        /// <summary>
        /// Formats and writes an ANSI sequence to a terminal output stream.
        /// </summary>
        /// <param name="format">The format to be used. It accepts the same specifiers as the <c>std::println</c> function family.</param>
        /// <param name="arguments">The arguments to be formatted.</param>
        /// <typeparam name="Arguments">A parameter pack containing the arguments to be formatted.</typeparam>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="StreamRedirectionException">Thrown when all the output streams are redirected.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="FormatException">Thrown when the formatting requested is badly formed.</exception>
        template <typename... Arguments>
        static void WriteANSI(const std::string_view& format, Arguments... arguments)
        {
            /* NOTE: it uses a stream preference algorithm to avoid caching and flushing performance costs whenever possible. */
            Init();
            constexpr const char* redirectionFailMessage = "cannot write ANSI sequence due to all terminal output streams being redirected.";
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
                    throw StreamRedirectionException(redirectionFailMessage);
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
                    throw StreamRedirectionException(redirectionFailMessage);
                }
            }
        }

    public:
        Terminal() = delete;
        /// <summary>
        /// Gets the arguments given by the calling process to the current one.
        /// </summary>
        /// <returns>A vector containing the arguments.</returns>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated for the arguments.</exception>
        /// <exception cref="IOException">Thrown, on Linux, if the <c>/proc/self/cmdline</c> file cannot be opened for reading.</exception>
        static std::vector<UnicodeString> GetArguments();
        /// <summary>
        /// Checks whether the terminal input stream is being redirected.
        /// </summary>
        /// <returns>A boolean that states that.</returns>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        [[nodiscard]]
        static bool IsInputRedirected();
        /// <summary>
        /// Checks whether the terminal output stream is being redirected.
        /// </summary>
        /// <returns>A boolean that states that.</returns>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        [[nodiscard]]
        static bool IsOutputRedirected();
        /// <summary>
        /// Checks whether the terminal error stream is being redirected.
        /// </summary>
        /// <returns>A boolean that states that.</returns>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        [[nodiscard]]
        static bool IsErrorRedirected();
        /// <summary>
        /// Flushes the terminal output stream buffer, writing any content it has to the stream.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to be flushed or when the operation fails.</exception>
        static void FlushOutput();
        /// <summary>
        /// Flushes the terminal input buffer, losing any content it has cached.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to be flushed or when the operation fails.</exception>
        /// <exception cref="InternalAttributesException">Thrown, on macOS and Linux, when internal attributes of the stream could not be fetched or set.</exception>
        static void FlushInput();
        /// <summary>
        /// Specifies whether the terminal should use custom styles including colors and text styles. By default, it allows it if the environment variable <c>NO_COLOR</c> is not set and, on macOS and Linux, when the environment variable <c>TERM</c> is not <c>dumb</c>.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        static void SetStylesUse(bool allowsTextStyle);
        /// <summary>
        /// Writes the ANSI sequence that sets a 256-color ANSI palette color in the terminal foreground to the most viable output stream.
        /// </summary>
        /// <param name="ansiColor">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• Though rare, some limited terminals may not support setting colors above value 15, only including the standard support.</para>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        /// </remarks>
        static void SetForeground(std::uint8_t ansiColor);
        /// <summary>
        /// Writes the ANSI sequence that sets one of the first 16 standard colors of the ANSI palette in the terminal foreground to the most viable output stream.
        /// </summary>
        /// <param name="color">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        /// </remarks>
        static void SetForeground(ANSIColor color);
        /// <summary>
        /// Writes the ANSI sequence that sets an RGB color in the terminal foreground to the most viable output stream.
        /// </summary>
        /// <param name="red">The red component of the color.</param>
        /// <param name="green">The green component of the color.</param>
        /// <param name="blue">The blue component of the color.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• The terminal requires TrueColor support for this type of color to be visible, which most modern terminals provide.</para>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        /// </remarks>
        static void SetForeground(std::uint8_t red, std::uint8_t green, std::uint8_t blue);
        /// <summary>
        /// Writes the ANSI sequence that sets an RGB color in the terminal foreground to the most viable output stream.
        /// </summary>
        /// <param name="color">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• The terminal requires TrueColor support for this type of color to be visible, which most modern terminals provide.</para>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        /// </remarks>
        static void SetForeground(RGBColor color);
        /// <summary>
        /// Writes the ANSI sequence that sets a color of the 256 colors ANSI palette in the terminal background.
        /// </summary>
        /// <param name="ansiColor">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• Though rare, some limited terminals may not support setting colors above value 15, only including the standard support.</para>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        /// </remarks>
        static void SetBackground(std::uint8_t ansiColor);
        /// <summary>
        /// Writes the ANSI sequence that sets one of the first 16 standard colors of the ANSI palette in the terminal background to the most viable stream.
        /// </summary>
        /// <param name="color">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        /// </remarks>
        static void SetBackground(ANSIColor color);
        /// <summary>
        /// Writes the ANSI sequence that sets an RGB color in the terminal background to the most viable output stream.
        /// </summary>
        /// <param name="red">The red component of the color.</param>
        /// <param name="green">The green component of the color.</param>
        /// <param name="blue">The blue component of the color.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• The terminal requires TrueColor support for this type of color to be visible, which most modern terminals provide.</para>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        /// </remarks>
        static void SetBackground(std::uint8_t red, std::uint8_t green, std::uint8_t blue);
        /// <summary>
        /// Writes the ANSI sequence that sets an RGB color in the terminal background to the most viable stream.
        /// </summary>
        /// <param name="color">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• The terminal requires TrueColor support for this type of color to be visible, which most modern terminals provide.</para>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        /// </remarks>
        static void SetBackground(RGBColor color);
        /// <summary>
        /// Writes the ANSI sequence that sets a 256-color ANSI palette color in the terminal underline ornament to the most viable output stream.
        /// </summary>
        /// <param name="ansiColor">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• Though rare, some limited terminals may not support setting colors above value 15, only including the standard support.</para>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        ///     <para>• One of the underline styles, <c>TextStyle::Underline</c>, <c>TextStyle::DoubleUnderline</c>, or <c>TextStyle::SquigglyLine</c> style must be set.</para>
        /// </remarks>
        static void SetUnderlineColor(std::uint8_t ansiColor);
        /// <summary>
        /// Writes the ANSI sequence that sets one of the first 16 standard colors of the ANSI palette in the terminal ornament to the most viable stream.
        /// </summary>
        /// <param name="color">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• Though rare, some limited terminals may not support setting colors above value 15, only including the standard support.</para>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        ///     <para>• One of the underline styles, <c>TextStyle::Underline</c>, <c>TextStyle::DoubleUnderline</c>, or <c>TextStyle::SquigglyLine</c> style must be set.</para>
        /// </remarks>
        static void SetUnderlineColor(ANSIColor color);
        /// <summary>
        /// Writes the ANSI sequence that sets an RGB color in the terminal ornament to the most viable stream.
        /// </summary>
        /// <param name="red">The red component of the color.</param>
        /// <param name="green">The green component of the color.</param>
        /// <param name="blue">The blue component of the color.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• The terminal requires TrueColor support for this type of color to be visible, which most modern terminals provide.</para>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        ///     <para>• One of the underline styles, <c>TextStyle::Underline</c>, <c>TextStyle::DoubleUnderline</c>, or <c>TextStyle::SquigglyLine</c> style must be set.</para>
        /// </remarks>
        static void SetUnderlineColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue);
        /// <summary>
        /// Writes the ANSI sequence that sets an RGB color in the terminal ornament to the most viable stream.
        /// </summary>
        /// <param name="color">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• The terminal requires TrueColor support for this type of color to be visible, which most modern terminals provide.</para>
        ///     <para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        ///     <para>• One of the underline styles, <c>TextStyle::Underline</c>, <c>TextStyle::DoubleUnderline</c>, or <c>TextStyle::SquigglyLine</c> style must be set.</para>
        /// </remarks>
        static void SetUnderlineColor(RGBColor color);
        /// <summary>
        /// Writes the ANSI sequences that sets the terminal text styles flagged in a bitmask to the most viable stream.
        /// </summary>
        /// <param name="styles">The bitmask containing the styles. It must be composed by using the <c>|</c> (bitwise OR) operator between members of the enum class <c>TextStyle</c>.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• Though rare, some terminals may not support all the styles available, causing them to not take effect.</para>
        ///     <para>• If all output streams are being redirected, the styles are not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetTextStyles</c> function must be made as the shell may not reset them automatically after the current process execution.</para>
        /// </remarks>
        static void SetTextStyles(int styles);
        /// <summary>
        /// Writes the ANSI sequence that sets a terminal text style to the most viable stream.
        /// </summary>
        /// <param name="style">The style to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>
        ///     <para>• Though rare, some terminals may not support all the styles available, causing them to not take effect.</para>
        ///     <para>• If all output streams are being redirected, the style is not applied, not throwing exceptions or errors.</para>
        ///     <para>• If all output streams are being redirected, the styles are not applied, not throwing exceptions or errors.</para>
        ///     <para>• In order to reset it, a call to the <c>ResetTextStyles</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        ///  </remarks>
        static void SetTextStyles(TextStyle style);
        /// <summary>
        /// Writes the ANSI sequences that resets the terminal foreground and background colors to the most viable stream.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        static void ResetColors();
        /// <summary>
        /// Writes the ANSI sequences that resets all terminal text styles to the most viable stream.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        static void ResetTextStyles();
        /// <summary>
        /// Writes the ANSI sequence that opens the terminal alternate screen to the most viable output stream.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="StreamRedirectionException">Thrown when all terminal output streams are redirected.</exception>
        /// <remarks>
        ///     <para>• Opening the alternate screen while it is already opened, resets it in place, allowing you to perform repainting.</para>
        ///     <para>• Though rare, some terminals may not have support to this feature. In that case, their screens are simply cleared, giving a similar effect.</para>
        ///     <para>• In order to close it, a call to  the <c>CloseAlternateScreen</c> function must be made as the shell may not reset it automatically after the current process execution.</para>
        /// </remarks>
        static void OpenAlternateScreen();
        /// <summary>
        /// Writes the ANSI sequence that closes the terminal alternate screen to the most viable output stream.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="StreamRedirectionException">Thrown when all terminal output streams are redirected.</exception>
        static void CloseAlternateScreen();
        /// <summary>
        /// Gets the terminal screen dimensions.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="StreamRedirectionException">Thrown when all streams are being redirected.</exception>
        /// <exception cref="InternalAttributesException">Thrown when the dimensions cannot be obtained due to an operating system error.</exception>
        /// <remarks>The operating system kernel or console submodule can report dimensions that may not match the graphical window or monitor sizes.</remarks>
        static Dimensions GetDimensions();
        /// <summary>
        /// Gets the terminal cursor coordinate either via a low-level call, on Windows, or by requesting and parsing a terminal response, on macOS and Linux, via to write of an ANSI sequence to the most viable stream.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown, on macOS and Linux, when the input stream or the viable output stream does not have a buffer to perform caching or when the write operation of the request fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown, on macOS and Linux, when not enough memory can be allocated to format the request.</exception>
        /// <exception cref="StreamRedirectionException">Thrown, on macOS and Linux, when the terminal input stream or the output streams are redirected, not allowing the request and read of the response.</exception>
        /// <exception cref="FormatException">Thrown, on macOS and Linux, when the terminal response is badly formed.</exception>
        /// <remarks>
        ///     <para>• It considers a coordinate system where the origin point is in the top left corner of the terminal screen. From there, the column (in the horizontal axis) and row (in the vertical axis) values increase going right and down respectively.</para>
        ///     <para>• On macOS and Linux, the coordinate is obtained by parsing a response given by the terminal through the terminal input stream, requiring it to be flushed upon execution to ensure correctness.</para>
        /// </remarks>
        static Coordinate GetCursorCoordinate();
        /// <summary>
        /// Writes the ANSI sequence that sets the terminal cursor coordinate to the most viable stream.
        /// </summary>
        /// <param name="column">The column component of the coordinate to be set.</param>
        /// <param name="row">The row component of the coordinate to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="StreamRedirectionException">Thrown when all terminal output streams are redirected.</exception>
        /// <exception cref="InternalAttributesException">Thrown when the terminal dimensions cannot be obtained due to an operating system error.</exception>
        /// <exception cref="OutOfRangeException">Thrown when the coordinate provided is not contained within the terminal dimensions.</exception>
        /// <remarks>It considers a coordinate system where the origin point is in the top left corner of the terminal screen. From there, the column (in the horizontal axis) and row (in the vertical axis) values increase going right and down respectively.</remarks>
        static void SetCursorCoordinate(std::uint16_t column, std::uint16_t row);
        /// <summary>
        /// Writes the ANSI sequence that sets the terminal cursor coordinate to the most viable stream.
        /// </summary>
        /// <param name="coordinate">The coordinate to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="StreamRedirectionException">Thrown when all terminal output streams are redirected.</exception>
        /// <exception cref="InternalAttributesException">Thrown when the terminal dimensions cannot be obtained due to an operating system error.</exception>
        /// <exception cref="OutOfRangeException">Thrown when the coordinate provided is not contained within the terminal dimensions.</exception>
        /// <remarks>It considers a coordinate system where the origin point is in the top left corner of the terminal screen. From there, the column (in the horizontal axis) and row (in the vertical axis) values increase going right and down respectively.</remarks>
        static void SetCursorCoordinate(const Coordinate& coordinate);
        /// <summary>
        /// Writes the ANSI sequence that moves the terminal cursor in a given direction.
        /// </summary>
        /// <param name="steps">The number of steps to take.</param>
        /// <param name="direction">The direction of the movement.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="StreamRedirectionException">Thrown when all terminal output streams are redirected.</exception>
        /// <exception cref="InternalAttributesException">Thrown when the terminal dimensions cannot be obtained due to an operating system error.</exception>
        /// <exception cref="OutOfRangeException">Thrown when the final cursor coordinate after the movement is not contained within the terminal dimensions.</exception>
        static void MoveCursor(std::uint16_t steps, Direction direction);
        /// <summary>
        /// Writes the ANSI sequence that sets the terminal cursor visibility.
        /// </summary>
        /// <param name="isVisible">A boolean that states the cursor should be visible.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        static void SetCursorVisible(bool isVisible);
        /// <summary>
        /// Writes the ANSI sequence that sets a terminal cursor style.
        /// </summary>
        /// <param name="style">The style to apply.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        static void SetCursorStyle(CursorStyle style);
        /// <summary>
        /// Writes the ANSI sequence that resets the cursor style.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        static void ResetCursorStyle();
        /// <summary>
        /// Writes the ANSI sequence that rings the terminal bell.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks>The terminal bell behavior depends on configuration and may be disabled, use custom sounds, visual effects, or system notifications.</remarks>
        static void RingBell();
        /// <summary>
        /// Writes the ANSI sequences that clears the terminal screen and moves the cursor to the top left corner.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        static void ClearScreen();
        /// <summary>
        /// Writes the ANSI sequences that clears the terminal cursor line and moves the cursor to its first column.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        static void ClearLine();
        /// <summary>
        /// Writes the ANSI sequences that clears the scrollback buffer and moves the cursor to the top left corner of the screen.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        static void ClearHistory();
        // static bool HasInput(); // checks whether the input buffer has events.
        // static void SetTitle(const std::string_view& title);
        // static std::string CreateURL(const std::string_view& format, Arguments... arguments); // <-- possibly better to create a URL class with custom formatting

        /// <summary>
        /// Formats and writes a string to the terminal output stream.
        /// </summary>
        /// <param name="format">The format to be used. It accepts the same specifiers as the <c>std::println</c> function family.</param>
        /// <param name="arguments">The arguments to be formatted.</param>
        /// <typeparam name="Arguments">A parameter pack containing the arguments to be formatted.</typeparam>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="FormatException">Thrown when the formatting requested is badly formed.</exception>
        template <typename... Arguments>
        static void Write(const std::string_view& format, Arguments... arguments)
        {
            Init();
            if (!std::cout.rdbuf())
            {
                throw IOException("the terminal output stream does not a buffer to perform caching.");
            }
            std::string result;
            try
            {
                result = std::vformat(format, std::make_format_args(arguments...));
            }
            catch (std::format_error&)
            {
                throw FormatException("the formatting requested for output is badly formed.");
            }
            catch (...)
            {
                throw NotEnoughMemoryException("cannot allocate enough memory to format the output.");
            }
            std::cout.clear();
            constexpr const char* writeFailMessage = "cannot write to the terminal output stream.";
            try
            {
                std::cout << result;
            }
            catch (...)
            {
                std::cout.clear();
                throw IOException(writeFailMessage);
            }
            if (std::cout.fail())
            {
                std::cout.clear();
                throw IOException(writeFailMessage);
            }
            std::cout.clear();
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
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="FormatException">Thrown when the formatting requested is badly formed.</exception>
        template <typename T>
        static void Write(const T& argument)
        {
            Write("{}", argument);
        }

        /// <summary>
        /// Formats and writes a string to the terminal output stream with a newline sequence appended to its end.
        /// </summary>
        /// <param name="format">The format to be used. It accepts the same specifiers as the <c>std::println</c> function family.</param>
        /// <param name="arguments">The arguments to be formatted.</param>
        /// <typeparam name="Arguments">A parameter pack containing the arguments to be formatted.</typeparam>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="FormatException">Thrown when the formatting requested is badly formed.</exception>
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
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="FormatException">Thrown when the formatting requested is badly formed.</exception>
        template <typename T>
        static void WriteLine(const T& argument)
        {
            WriteLine("{}", argument);
        }

        /// <summary>
        /// Formats and writes a string to the terminal error stream.
        /// </summary>
        /// <param name="format">The format to be used. It accepts the same specifiers as the <c>std::println</c> function family.</param>
        /// <param name="arguments">The arguments to be formatted.</param>
        /// <typeparam name="Arguments">A parameter pack containing the arguments to be formatted.</typeparam>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="FormatException">Thrown when the formatting requested is badly formed.</exception>
        template <typename... Arguments>
        static void WriteError(const std::string_view& format, Arguments... arguments)
        {
            Init();
            if (s_hasANSICache)
            {
                FlushOutput();
            }
            if (!std::cerr.rdbuf())
            {
                throw IOException("the terminal error stream does not a buffer to perform caching.");
            }
            std::string result;
            try
            {
                result = std::vformat(format, std::make_format_args(arguments...));
            }
            catch (std::format_error&)
            {
                throw FormatException("the formatting requested for output is badly formed.");
            }
            catch (...)
            {
                throw NotEnoughMemoryException("cannot allocate enough memory to format the output.");
            }
            std::cerr.clear();
            constexpr const char* writeFailMessage = "cannot write to the terminal error stream.";
            try
            {
                std::cerr << result;
            }
            catch (...)
            {
                std::cerr.clear();
                throw IOException(writeFailMessage);
            }
            if (std::cerr.fail())
            {
                std::cerr.clear();
                throw IOException(writeFailMessage);
            }
            std::cerr.clear();
            s_ansiPrefersStdOut = false;
        }

        /// <summary>
        /// Formats and writes an argument to the terminal error stream.
        /// </summary>
        /// <param name="argument">The argument to be formatted.</param>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="FormatException">Thrown when the formatting requested is badly formed.</exception>
        template <typename T>
        static void WriteError(const T& argument)
        {
            WriteError("{}", argument);
        }

        /// <summary>
        /// Formats and writes a string to the terminal error stream with a newline sequence appended to its end.
        /// </summary>
        /// <param name="format">The format to be used. It accepts the same specifiers as the <c>std::println</c> function family.</param>
        /// <param name="arguments">The arguments to be formatted.</param>
        /// <typeparam name="Arguments">A parameter pack containing the arguments to be formatted.</typeparam>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="FormatException">Thrown when the formatting requested is badly formed.</exception>
        template <typename... Arguments>
        static void WriteErrorLine(const std::string_view& format, Arguments... arguments)
        {
            WriteError(format, arguments...);
            WriteError("\n");
        }

        /// <summary>
        /// Formats and writes an argument to the terminal error stream with a newline appended to its end.
        /// </summary>
        /// <param name="argument">The argument to be formatted.</param>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="FormatException">Thrown when the formatting requested is badly formed.</exception>
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
