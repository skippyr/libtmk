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

    class NotEnoughMemoryException final : public Exception
    {
    public:
        NotEnoughMemoryException(const std::string_view& message);
    };

#ifdef _WIN32
    class BadEncodingException final : public Exception
    {
    public:
        BadEncodingException(const std::string_view& message);
    };
#endif

    class IOException : public Exception
    {
    public:
        IOException(const std::string_view& message);
    };

    class InternalAttributesException : public Exception
    {
    public:
        InternalAttributesException(const std::string_view& message);
    };

    class FormatException : public Exception
    {
    public:
        FormatException(const std::string_view& message);
    };

    class StreamRedirectionException final : public IOException
    {
    public:
        StreamRedirectionException(const std::string_view& message);
    };

    class OutOfRangeException final : public Exception
    {
    public:
        OutOfRangeException(const std::string_view& message);
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
        /// A boolean that states the terminal features have been initialized.
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
        /// Initiates the required terminal features. It includes setting up UTF-8 as the output encoding, enable the parse of ANSI escape sequences,
        /// and cache metadata about streams and style allowance.
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
        /// Sets the bitmask flag that enables the parse of ANSI sequences in a terminal output stream mode. It is only available on Windows.
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
        /// <exception cref="StreamRedirectionException">Thrown when all possible streams that could return an answer are being redirected.</exception>
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
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <exception cref="FormatException">Thrown when the formatting requested is badly formed.</exception>
        template <typename... Arguments>
        static void WriteAnsi(const std::string_view& format, Arguments... arguments)
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
        /// Gets the arguments given by the terminal shell to the current process.
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
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to be flushed or when the operation fails.</exception>
        static void FlushOutput();
        /// <summary>
        /// Flushes the terminal input buffer, losing any content it has cached.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initialized.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to be flushed or when the operation fails.</exception>
        /// <exception cref="InternalAttributesException">Thrown, on macOS and Linux, when internal attributes of the stream could not be fetched or set.</exception>
        static void FlushInput();
        /// <summary>
        /// Specifies whether the terminal should use custom styles including colors and text styles. By default, it allows it if the environment variable <c>NO_COLOR</c> is
        /// not set and, on macOS and Linux, when the environment variable <c>TERM</c> is not <c>dumb</c>.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initialized.</exception>
        static void SetStylesUse(bool allowsTextStyle);
        /// <summary>
        /// Writes the ANSI sequence that sets a 256-color ANSI palette color in the terminal foreground to the most viable output stream.
        /// </summary>
        /// <param name="ansiColor">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks><para>• Though rare, some limited terminals may not support setting colors above value 15, only including the standard support.</para><para>• If all output
        /// streams are being redirected, the color is not applied, not throwing exceptions or errors.</para><para>• In order to reset it, a call to the <c>ResetColors</c> function
        /// must be made as the shell may not reset it automatically after the current process execution.</para></remarks>
        static void SetForeground(std::uint8_t ansiColor);
        /// <summary>
        /// Writes the ANSI sequence that sets one of the first 16 standard colors of the ANSI palette in the terminal foreground to the most viable output stream.
        /// </summary>
        /// <param name="color">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks><para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para><para>• In order to reset it, a call to
        /// the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para></remarks>
        static void SetForeground(ANSIColor color);
        /// <summary>
        /// Writes the ANSI sequence that sets an RGB color in the terminal foreground to the most viable output stream.
        /// </summary>
        /// <param name="color">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks><para>• The terminal requires TrueColor support for this type of color to be visible, which most modern terminals provide.</para><para>• If all output streams
        /// are being redirected, the color is not applied, not throwing exceptions or errors.</para><para>• In order to reset it, a call to the <c>ResetColors</c> function must be
        /// made as the shell may not reset it automatically after the current process execution.</para></remarks>
        static void SetForeground(RGBColor color);
        /// <summary>
        /// Writes the ANSI sequence that sets a color of the 256 colors ANSI palette in the terminal background.
        /// </summary>
        /// <param name="ansiColor">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks><para>• Though rare, some limited terminals may not support setting colors above value 15, only including the standard support.</para><para>• If all output
        /// streams are being redirected, the color is not applied, not throwing exceptions or errors.</para><para>• In order to reset it, a call to the <c>ResetColors</c> function
        /// must be made as the shell may not reset it automatically after the current process execution.</para></remarks>
        static void SetBackground(std::uint8_t ansiColor);
        /// <summary>
        /// Writes the ANSI sequence that sets one of the first 16 standard colors of the ANSI palette in the terminal background to the most viable stream.
        /// </summary>
        /// <param name="color">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks><para>• If all output streams are being redirected, the color is not applied, not throwing exceptions or errors.</para><para>• In order to reset it, a call to
        /// the <c>ResetColors</c> function must be made as the shell may not reset it automatically after the current process execution.</para></remarks>
        static void SetBackground(ANSIColor color);
        /// <summary>
        /// Writes the ANSI sequence that sets an RGB color in the terminal background to the most viable stream.
        /// </summary>
        /// <param name="color">The color to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks><para>• The terminal requires TrueColor support for this type of color to be visible, which most modern terminals provide.</para><para>• If all output streams
        /// are being redirected, the color is not applied, not throwing exceptions or errors.</para><para>• In order to reset it, a call to the <c>ResetColors</c> function must be
        /// made as the shell may not reset it automatically after the current process execution.</para></remarks>
        static void SetBackground(RGBColor color);
        /// <summary>
        /// Writes the ANSI sequences that sets the terminal text styles flagged in a bitmask to the most viable stream.
        /// </summary>
        /// <param name="styles">The bitmask containing the styles. It must be composed by using the <c>|</c> (bitwise OR) operator between members of the enum class
        /// <c>TextStyle</c>.</param> <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception> <exception cref="IOException">Thrown when
        /// the stream does not have a buffer to perform caching or when the write operation fails.</exception> <exception cref="NotEnoughMemoryException">Thrown  when not enough
        /// memory can be allocated to format the output.</exception> <remarks><para>• Though rare, some terminals may not support all the styles available, causing them to not
        /// take effect.</para><para>• If all output streams are being redirected, the styles are not applied, not throwing exceptions or errors.</para><para>• In order to reset
        /// it, a call to the <c>ResetTextStyles</c> function must be made as the shell may not reset them automatically after the current process execution.</para></remarks>
        static void SetTextStyles(int styles);
        /// <summary>
        /// Writes the ANSI sequence that sets a terminal text style to the most viable stream.
        /// </summary>
        /// <param name="style">The style to be set.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="NotEnoughMemoryException">Thrown when not enough memory can be allocated to format the output.</exception>
        /// <remarks><para>• Though rare, some terminals may not support all the styles available, causing them to not take effect.</para><para>• If all output streams are being
        /// redirected, the style is not applied, not throwing exceptions or errors.</para><para>• If all output streams are being redirected, the styles are not applied, not
        /// throwing exceptions or errors.</para><para>• In order to reset it, a call to the <c>ResetTextStyles</c> function must be made as the shell may not reset it
        /// automatically after the current process execution.</para></remarks>
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
        /// <remarks><para>• Opening the alternate screen while it is already opened, resets it in place, allowing you to perform repainting.</para><para>• Though rare, some
        /// terminals may not have support to this feature. In that case, their screens are simply cleared, giving a similar effect.</para><para>• In order to close it, a call to
        /// the <c>CloseAlternateScreen</c> function must be made as the shell may not reset it automatically after the current process execution.</para></remarks>
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
        /// Gets the terminal cursor coordinate either via a low-level call, on Windows, or by requesting and parsing a terminal response, on macOS and Linux, via the write of an
        /// ANSI sequence to the most viable stream.
        /// </summary>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown, on macOS and Linux, when the input stream or the viable output stream does not have a buffer to perform caching or when the write
        /// operation of the request fails.</exception> <exception cref="NotEnoughMemoryException">Thrown, on macOS and Linux, when not enough memory can be allocated to format the
        /// request.</exception> <exception cref="StreamRedirectionException">Thrown, on macOS and Linux, when the terminal input stream or the output streams are redirected, not
        /// allowing the request and read of the response.</exception> <exception cref="FormatException">Thrown, on macOS and Linux, when the terminal response is badly
        /// formatted.</exception> <remarks><para>• It considers a coordinate system where the origin point is in the top left corner of the terminal screen. From there, the column
        /// (in the horizontal axis) and row (in the vertical axis) values increase going right and down respectively.</para><para>• On macOS and Linux, the coordinate is obtained
        /// by parsing a response given by the terminal through the terminal input stream, requiring it to be flushed upon execution to ensure correctness.</para></remarks>
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
        /// <remarks>It considers a coordinate system where the origin point is in the top left corner of the terminal screen. From there, the column (in the horizontal axis) and
        /// row (in the vertical axis) values increase going right and down respectively.</remarks>
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
        /// <remarks>It considers a coordinate system where the origin point is in the top left corner of the terminal screen. From there, the column (in the horizontal axis) and
        /// row (in the vertical axis) values increase going right and down respectively.</remarks>
        static void SetCursorCoordinate(const Coordinate& coordinate);
        /// <summary>
        /// Writes the ANSI sequence that moves the terminal cursor to a specific direction.
        /// </summary>
        /// <param name="steps">The number of steps to take.</param>
        /// <param name="direction">The direction of the movement.</param>
        /// <exception cref="InitException">Thrown when the terminal features cannot be initiated.</exception>
        /// <exception cref="IOException">Thrown when the stream does not have a buffer to perform caching or when the write operation fails.</exception>
        /// <exception cref="StreamRedirectionException">Thrown when all terminal output streams are redirected.</exception>
        /// <exception cref="InternalAttributesException">Thrown when the terminal dimensions cannot be obtained due to an operating system error.</exception>
        /// <exception cref="OutOfRangeException">Thrown when the final cursor coordinate after the movement is not contained within the terminal dimensions.</exception>
        static void MoveCursor(std::uint16_t steps, Direction direction);
        static void SetCursorVisible(bool isVisible);
        static void SetCursorStyle(CursorStyle style);
        static void ResetCursorStyle();
        static void RingBell();
        static void ClearScreen();
        static void ClearLine();
        static void ClearHistory();
        // static bool HasInput(); // checks whether the input buffer has events.
        // static void SetTitle(const std::string_view& title);
        // static std::string CreateURL(const std::string_view& address); // <-- possibly better to create a URL class with custom formatting

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
