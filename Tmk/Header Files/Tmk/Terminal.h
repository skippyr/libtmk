#pragma once

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
                GetCppStream() << std::vformat(format, std::make_format_args(arguments...));
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
