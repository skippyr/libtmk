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
        Terminal() = delete;

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
            StreamRedirectionCache(bool isInputRedirected, bool isOutputRedirected, bool isErrorRedirected);
            /// <summary>
            /// Checks if a stream is redirected.
            /// </summary>
            /// <param name="fileNo">The file number related to the stream.</param>
            /// <return>A boolean that states the stream is redirected.</return>
            bool IsRedirected(int fileNo) const;
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

            Driver() = delete;

#if defined(_WIN32)
            /// <summary>
            /// Sets UTF-8 as the output encoding.
            /// </summary>
            static void SetUtf8Encoding();
            /// <summary>
            /// Enables the parse of ANSI escape sequences by setting the ENABLE_VIRTUAL_TERMINAL_PROCESSING flag.
            /// </summary>
            static void EnableVirtualTerminalProcessing();
#endif

        public:
            /// <summary>
            /// Gets the stream redirection cache.
            /// </summary>
            /// <return>The stream redirection cache.</return>
            static const StreamRedirectionCache& GetStreamRedirectionCache();
            /// <summary>
            /// Enables the driver features.
            /// </summary>
            static void EnableFeatures();
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
                if (!Output::IsRedirected())
                {
                    Output::Write(format, arguments...);
                }
                else if (!Error::IsRedirected())
                {
                    Error::Write(format, arguments...);
                }
                else
                {
                    throw new StreamRedirectionException("could not write ANSI escape sequence due to the terminal output and error streams being redirected.");
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
        private:
            Stream() = delete;

        protected:
            /// <summary>
            /// Gets the file number related to the stream.
            /// </summary>
            static int GetFileNo()
            {
                return N;
            }

        public:
            /// <summary>
            /// Checks if the stream is redirected.
            /// </summary>
            /// <return>A boolean that states the stream is redirected.</return>
            static bool IsRedirected()
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
            /// Gets the CPP output stream related to the stream.
            /// </summary>
            static std::ostream& GetCppStream()
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
        /// <summary>
        /// Represents the terminal input stream.
        /// </summary>
        class Input final : public Stream<0>
        {
            /// <summary>
            /// Clears the input buffer, removing possible cached key events that are inside of it.
            /// </summary>
            static void ClearBuffer();
        };

        /// <summary>
        /// Represents the terminal output stream.
        /// </summary>
        class Output final : public WritableStream<1>
        {
        public:
            /// <summary>
            /// Flushes the contents that are inside of the output buffer, writing them out.
            /// </summary>
            static void FlushBuffer();
        };

        /// <summary>
        /// Represents the terminal error stream.
        /// </summary>
        class Error final : public WritableStream<2>
        {
        };

        /// <summary>
        /// Represents the terminal font.
        /// </summary>
        class Font final
        {
        private:
            Font() = delete;

        public:
            /// <summary>
            /// Sets an ANSI color to a layer.
            /// </summary>
            /// <param name="color">The color to be applied.</param>
            /// <param name="layer">The layer to be affected.</param>
            static void SetColor(AnsiColor color, Layer layer);
            /// <summary>
            /// Sets an RGB color to a layer.
            /// </summary>
            /// <param name="color">The color to be applied.</param>
            /// <param name="layer">The layer to be affected.</param>
            static void SetColor(RgbColor color, Layer layer);
            /// <summary>
            /// Resets the font colors.
            /// </summary>
            static void ResetColors();
            /// <summary>
            /// Sets the font weight.
            /// </summary>
            /// <param name="weight">The weight to be set.</param>
            static void SetWeight(FontWeight weight);
            /// <summary>
            /// Resets the font weight.
            /// </summary>
            static void ResetWeight();
            /// <summary>
            /// Sets a font effect.
            /// </summary>
            /// <param name="effect">The effect to be set.</param>
            static void SetEffects(FontEffect effect);
            /// <summary>
            /// Sets the font effects flagged in a bitmask.
            /// </summary>
            /// <param name="effects">A bitmask containing the effects to be set.</param>
            static void SetEffects(int effects);
            /// <summary>
            /// Resets the font effects.
            /// </summary>
            static void ResetEffects();
        };

        /// <summary>
        /// Represents the terminal window.
        /// </summary>
        class Window final
        {
        private:
            Window() = delete;

        public:
            /// <summary>
            /// Gets the dimensions of the window.
            /// </summary>
            /// <return>The dimensions of the window.</return>
            /// <exception cref="StreamRedirectionException">Thrown when the possible data source streams are redirected.</exception>
            static Dimensions GetDimensions();
            /// <summary>
            /// Opens the alternate window.
            /// </summary>
            static void OpenAlternate();
            /// <summary>
            /// Closes the alternate window.
            /// </summary>
            static void CloseAlternate();
        };

        /// <summary>
        /// Represents the terminal cursor.
        /// </summary>
        class Cursor final
        {
        private:
            Cursor() = delete;

        public:
            /// <summary>
            /// Sets the cursor shape.
            /// </summary>
            /// <param name="shape">The shape to be set.</param>
            /// <param name="isBlinking">A boolean that states the cursor should blink.</param>
            static void SetShape(CursorShape shape, bool isBlinking);
            /// <summary>
            /// Resets the cursor shape.
            static void ResetShape();
            /// <summary>
            /// Sets the cursor visibility.
            /// </summary>
            /// <param name="isVisible">A boolean that states the cursor should be visible.</param>
            static void SetVisible(bool isVisible);
            /// <summary>
            /// Clears the line the cursor is on.
            /// </summary>
            static void ClearLine();
        };

        /// <summary>
        /// Represents the terminal bell.
        /// </summary>
        class Bell final
        {
        private:
            Bell() = delete;

        public:
            /// <summary>
            /// Rings the bell, possibly emitting a sound, system notification, screen flash and/or a bell symbol.
            /// </summary>
            static void Ring();
        };
    };
}
