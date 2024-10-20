#pragma once

#include <Tmk/System.h>

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
        /// Represents a cache of the stream redirection statuses.
        /// </summary>
        class StreamRedirectionCache final
        {
        private:
            /// <summary>
            /// A boolean that states the input stream is being redirected.
            /// </summary>
            bool m_isInputRedirected;
            /// <summary>
            /// A boolean that states the output stream is being redirected.
            /// </summary>
            bool m_isOutputRedirected;
            /// <summary>
            /// A boolean that states the error stream is being redirected.
            /// </summary>
            bool m_isErrorRedirected;

        public:
            /// <summary>
            /// Creates a new instance of the StreamRedirectionCache class with all streams being considered directed.
            /// </summary>
            StreamRedirectionCache();
            /// <summary>
            /// Creates a new instance of the StreamRedirectionCache class with the given statuses.
            /// </summary>
            /// <param name="isInputRedirected">A boolean that states the input stream is being redirected.</param>
            /// <param name="isOutputRedirected">A boolean that states the output stream is being redirected.</param>
            /// <param name="isErrorRedirected">A boolean that states the error stream is being redirected.</param>
            StreamRedirectionCache(bool isInputRedirected, bool isOutputRedirected, bool isErrorRedirected);
            /// <summary>
            /// Checks if a stream is being redirected.
            /// </summary>
            /// <param name="fileNo">The file number related to the stream.</param>
            /// <return>A boolean that states the stream is being redirected.</return>
            bool IsRedirected(int fileNo);
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
            /// A boolean that states the driver has enabled the terminal features.
            /// </summary>
            static bool s_hasEnabledFeatures;

            Driver() = delete;

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
            /// <summary>
            /// Sets UTF-8 as the terminal output encoding.
            /// </summary>
            static void SetUtf8Encoding();
            /// <summary>
            /// Enables the parse of ANSI escape sequences by setting the ENABLE_VIRTUAL_TERMINAL_PROCESSING flag.
            /// </summary>
            static void EnableVirtualTerminalProcessing();
#endif

        public:
            /// <summary>
            /// Enables the terminal features.
            /// </summary>
            static void EnableFeatures();
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

        public:
            /// <summary>
            /// Gets the file number related to the stream.
            /// </summary>
            static int GetFileNo()
            {
                return N;
            }
        };

        /// <summary>
        /// Represents a writable terminal stream.
        /// </summary>
        /// <typeparam name="N">The file number related to the stream.</typeparam>
        template <int N>
        class WritableStream
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
            /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
            /// <param name="arguments">The arguments to be formatted.</param>
            /// <typeparam name="Args">A parameter pack containing the arguments to be formatted.</typeparam>
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
            /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
            /// <param name="arguments">The arguments to be formatted.</param>
            /// <typeparam name="Args">A parameter pack containing the arguments to be formatted.</typeparam>
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
        };

        /// <summary>
        /// Represents the terminal output stream.
        /// </summary>
        class Output final : public WritableStream<1>
        {
        };

        /// <summary>
        /// Represents the terminal error stream.
        /// </summary>
        class Error final : public WritableStream<2>
        {
        };
    };
}
