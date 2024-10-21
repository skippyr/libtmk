#pragma once

#include <Tmk/AnsiColor.h>
#include <Tmk/CursorShape.h>
#include <Tmk/Dimensions.h>
#include <Tmk/FontEffect.h>
#include <Tmk/FontWeight.h>
#include <Tmk/Layer.h>
#include <Tmk/RgbColor.h>
#include <Tmk/StreamRedirectionException.h>
#include <format>
#include <iostream>

namespace Tmk
{
    /// <summary>
    /// Represents the terminal emulator.
    /// </summary>
    class Terminal final
    {
    private:
        /// <summary>
        /// Represents a cache of the terminal stream redirection statuses.
        /// </summary>
        class StreamRedirectionCache final
        {
        private:
            /// <summary>
            /// A boolean that states the input stream is redirected.
            /// </summary>
            bool m_isInputRedirected;
            /// <summary>
            /// A boolean that states the output stream is redirected.
            /// </summary>
            bool m_isOutputRedirected;
            /// <summary>
            /// A boolean that states the error stream is redirected.
            /// </summary>
            bool m_isErrorRedirected;

        public:
            /// <summary>
            /// Creates an instance of the StreamRedirectionCache class with all streams unredirected.
            /// </summary>
            StreamRedirectionCache();
            /// <summary>
            /// Creates an instance of the StreamRedirectionCache class with the given statuses.
            /// </summary>
            /// <param name="isInputRedirected">A boolean that states the input stream is redirected.</param>
            /// <param name="isOutputRedirected">A boolean that states the output stream is redirected.</param>
            /// <param name="isErrorRedirected">A boolean that states the error stream is redirected.</param>
            StreamRedirectionCache(bool isInputRedirected, bool isOutputRedirected, bool isErrorRedirected) noexcept;
            /// <summary>
            /// Checks if a stream is redirected.
            /// </summary>
            /// <param name="fileNo">The file number related to the stream.</param>
            /// <returns>A boolean that states the stream is redirected.</returns>
            [[nodiscard]] bool IsRedirected(int fileNo) const noexcept;
        };

        /// <summary>
        /// Represents the terminal driver.
        /// </summary>
        class Driver final
        {
        private:
            /// <summary>
            /// A cache of the stream redirection statuses.
            /// </summary>
            static StreamRedirectionCache s_streamRedirectionCache;
            /// <summary>
            /// A boolean that states the driver has enabled its features.
            /// </summary>
            static bool s_hasEnabledFeatures;

#if defined(_WIN32)
            /// <summary>
            /// Sets UTF-8 as the output encoding.
            /// </summary>
            static void SetUtf8Encoding() noexcept;
            /// <summary>
            /// Enables the parse of ANSI escape sequences by setting the ENABLE_VIRTUAL_TERMINAL_PROCESSING flag.
            /// </summary>
            static void EnableVirtualTerminalProcessing() noexcept;
#endif

        public:
            Driver() = delete;

            /// <summary>
            /// Gets the stream redirection cache.
            /// </summary>
            /// <returns>The stream redirection cache.</returns>
            [[nodiscard]] static const StreamRedirectionCache& GetStreamRedirectionCache() noexcept;
            /// <summary>
            /// Enables the driver features.
            /// </summary>
            static void EnableFeatures() noexcept;
            /// <summary>
            /// Formats and writes an ANSI escape sequence to the output or error streams.
            /// </summary>
            /// <typeparam name="Args">A parameter pack containing the arguments to be formatted.</typeparam>
            /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
            /// <param name="arguments">The arguments to be formatted.</param>
            /// <exception cref="StreamRedirectionException">Thrown when both streams are redirected.</exception>
            template <typename... Args>
            static void WriteAnsiEscapeSequence(std::string_view format, Args... arguments)
            {
                if (!OutputStream::IsRedirected())
                {
                    OutputStream::Write(format, arguments...);
                }
                else if (!ErrorStream::IsRedirected())
                {
                    ErrorStream::Write(format, arguments...);
                }
                else
                {
                    throw StreamRedirectionException("could not write ANSI escape sequence due to the terminal output and error streams being redirected.");
                }
            }
        };

        /// <summary>
        /// Represents a terminal stream.
        /// </summary>
        /// <typeparam name="N">The file number related to the stream.</typeparam>
        template <int N>
        class Stream
        {
        protected:
            /// <summary>
            /// Gets the file number related to the stream.
            /// </summary>
            /// <returns>The file number related to the stream.</returns>
            [[nodiscard]] static int GetFileNo() noexcept
            {
                return N;
            }

        public:
            Stream() = delete;

            /// <summary>
            /// Checks if the stream is redirected.
            /// </summary>
            /// <returns>A boolean that states the stream is redirected.</returns>
            [[nodiscard]] static bool IsRedirected() noexcept
            {
                return Driver::GetStreamRedirectionCache().IsRedirected(GetFileNo());
            }
        };

        /// <summary>
        /// Represents a writable terminal stream.
        /// </summary>
        /// <typeparam name="N">The file number related to the stream.</typeparam>
        template <int N>
        class WritableStream : public Stream<N>
        {
        private:
            /// <summary>
            /// Gets the C++ output stream (ostream) related to the stream.
            /// </summary>
            /// <returns>The C++ output stream (ostream) related to the stream.</returns>
            [[nodiscard]] static std::ostream& GetCppStream() noexcept
            {
                return N == 1 ? std::cout : std::cerr;
            }

        public:
            /// <summary>
            /// Formats and writes a string to the stream.
            /// </summary>
            /// <typeparam name="Args">A parameter pack containing the arguments to be formatted.</typeparam>
            /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
            /// <param name="arguments">The arguments to be formatted.</param>
            template <typename... Args>
            static void Write(std::string_view format, Args... arguments)
            {
                Driver::EnableFeatures();
                GetCppStream() << std::vformat(format, std::make_format_args(arguments...));
            }

            /// <summary>
            /// Writes a newline to the stream.
            /// </summary>
            static void WriteLine()
            {
                Driver::EnableFeatures();
                GetCppStream() << std::endl;
            }

            /// <summary>
            /// Formats and writes a string to the stream with a newline append to its end.
            /// </summary>
            /// <typeparam name="Args">A parameter pack containing the arguments to be formatted.</typeparam>
            /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
            /// <param name="arguments">The arguments to be formatted.</param>
            template <typename... Args>
            static void WriteLine(std::string_view format, Args... arguments)
            {
                Write(format, arguments...);
                GetCppStream() << std::endl;
            }
        };

    public:
        Terminal() = delete;

        /// <summary>
        /// Represents the terminal input stream.
        /// </summary>
        class InputStream final : public Stream<0>
        {
            /// <summary>
            /// Clears the input buffer, removing possible cached key events that are inside it.
            /// </summary>
            static void ClearBuffer() noexcept;
        };

        /// <summary>
        /// Represents the terminal output stream.
        /// </summary>
        class OutputStream final : public WritableStream<1>
        {
        public:
            /// <summary>
            /// Flushes the contents that are inside the output buffer, writing them out.
            /// </summary>
            static void FlushBuffer() noexcept;
        };

        /// <summary>
        /// Represents the terminal error stream.
        /// </summary>
        class ErrorStream final : public WritableStream<2>
        {
        };

        /// <summary>
        /// Represents the terminal font.
        /// </summary>
        class Font final
        {
        public:
            Font() = delete;

            /// <summary>
            /// Sets an ANSI color to a layer.
            /// </summary>
            /// <param name="color">The color to be applied.</param>
            /// <param name="layer">The layer to be affected.</param>
            static void SetColor(AnsiColor color, Layer layer) noexcept;
            /// <summary>
            /// Sets an RGB color to a layer.
            /// </summary>
            /// <param name="color">The color to be applied.</param>
            /// <param name="layer">The layer to be affected.</param>
            static void SetColor(RgbColor color, Layer layer) noexcept;
            /// <summary>
            /// Resets the font colors.
            /// </summary>
            static void ResetColors() noexcept;
            /// <summary>
            /// Sets the font weight.
            /// </summary>
            /// <param name="weight">The weight to be set.</param>
            static void SetWeight(FontWeight weight) noexcept;
            /// <summary>
            /// Resets the font weight.
            /// </summary>
            static void ResetWeight() noexcept;
            /// <summary>
            /// Sets a font effect.
            /// </summary>
            /// <param name="effect">The effect to be set.</param>
            static void SetEffects(FontEffect effect) noexcept;
            /// <summary>
            /// Sets the font effects flagged in a bitmask.
            /// </summary>
            /// <param name="effects">A bitmask containing the effects to be set.</param>
            static void SetEffects(int effects) noexcept;
            /// <summary>
            /// Resets the font effects.
            /// </summary>
            static void ResetEffects() noexcept;
        };

        /// <summary>
        /// Represents the terminal window.
        /// </summary>
        class Window final
        {
        public:
            Window() = delete;

            /// <summary>
            /// Gets the dimensions of the window.
            /// </summary>
            /// <returns>The dimensions of the window.</returns>
            /// <exception cref="StreamRedirectionException">Thrown when the possible data source streams are redirected.</exception>
            [[nodiscard]] static Dimensions GetDimensions();
            /// <summary>
            /// Opens the alternate window.
            /// </summary>
            static void OpenAlternate() noexcept;
            /// <summary>
            /// Closes the alternate window.
            /// </summary>
            static void CloseAlternate() noexcept;
        };

        /// <summary>
        /// Represents the terminal cursor.
        /// </summary>
        class Cursor final
        {
        public:
            Cursor() = delete;

            /// <summary>
            /// Sets the cursor shape.
            /// </summary>
            /// <param name="shape">The shape to be set.</param>
            /// <param name="isBlinking">A boolean that states the cursor should blink.</param>
            static void SetShape(CursorShape shape, bool isBlinking) noexcept;
            /// <summary>
            /// Resets the cursor shape.
            /// </summary>
            static void ResetShape() noexcept;
            /// <summary>
            /// Sets the cursor visibility.
            /// </summary>
            /// <param name="isVisible">A boolean that states the cursor should be visible.</param>
            static void SetVisible(bool isVisible) noexcept;
            /// <summary>
            /// Clears the line the cursor is on.
            /// </summary>
            static void ClearLine() noexcept;
        };

        /// <summary>
        /// Represents the terminal bell.
        /// </summary>
        class Bell final
        {
        public:
            Bell() = delete;

            /// <summary>
            /// Rings the bell, possibly emitting a sound, system notification, screen flash and/or a bell symbol.
            /// </summary>
            static void Ring() noexcept;
        };
    };
}
