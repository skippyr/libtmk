#pragma once

#include "System.h"

#include <iostream>

namespace Tmk
{
    /// <summary>Represents the terminal emulator.</summary>
    class Terminal
    {
    private:
        /// <summary>A boolean that states the terminal features have been enabled.</summary>
        static bool s_hasEnabledFeatures;
        /// <summary>A boolean that states the input stream is redirected.</summary>
        static bool s_isInputRedirected;
        /// <summary>A boolean that states the output stream is redirected.</summary>
        static bool s_isOutputRedirected;
        /// <summary>A boolean that states the error stream is redirected.</summary>
        static bool s_isErrorRedirected;

        Terminal() = delete;

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        /// <summary>Sets UTF-8 as the output encoding on Windows.</summary>
        static void SetUtf8Encoding();
        /// <summary>Enables the parse of ANSI escape sequences on Windows.</summary>
        static void EnableVirtualTerminalProcessing();
#endif
        /// <summary>Initiates the stream redirection cache.</summary>
        static void InitStreamRedirectionCache();
        /// <summary>Enables the required terminal features.</summary>
        static void EnableFeatures();

        /// <summary>Writes an ANSI escape sequence to the standard output or error streams.
        /// <tparam name="Args">A parameter pack containing the arguments to be formatted.</param>
        /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
        /// <param name="...">The arguments to be formatted.</param>
        template <typename... Args>
        static void WriteAnsiEscapeSequence(std::string_view format, Args... arguments)
        {
            if (!IsOutputRedirected())
            {
                Write(format, arguments...);
            }
            else if (!IsErrorRedirected())
            {
                WriteError(format, arguments...);
            }
        }

    public:
        /// <summary>Checks if the standard input stream is redirected.</summary>
        /// <return>A boolean that states the stream is redirected.</summary>
        static bool IsInputRedirected();
        /// <summary>Checks if the standard output stream is redirected.</summary>
        /// <return>A boolean that states the stream is redirected.</summary>
        static bool IsOutputRedirected();
        /// <summary>Checks if the standard error stream is redirected.</summary>
        /// <return>A boolean that states the stream is redirected.</summary>
        static bool IsErrorRedirected();

        /// <summary>Formats and writes a string to the standard output stream.</summary>
        /// <tparam name="Args">A parameter pack containing the arguments to be formatted.</tparam>
        /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
        /// <param name="...">The arguments to be formatted.</param>
        template <typename... Args>
        static void Write(std::string_view format, Args... arguments)
        {
            EnableFeatures();
            std::cout << std::vformat(format, std::make_format_args(arguments...));
        }

        /// <summary>Writes a newline to the standard output stream.</summary>
        static void WriteLine();

        /// <summary>Formats and writes a string to the standard output stream with a newline appended to its end.</summary>
        /// <tparam name="Args">A parameter pack containing the arguments to be formatted.</tparam>
        /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
        /// <param name="...">The arguments to be formatted.</param>
        template <typename... Args>
        static void WriteLine(std::string_view format, Args... arguments)
        {
            Write(format, arguments...);
            std::cout << std::endl;
        }

        /// <summary>Formats and writes a string to the standard error stream.</summary>
        /// <tparam name="Args">A parameter pack containing the arguments to be formatted.</tparam>
        /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
        /// <param name="...">The arguments to be formatted.</param>
        template <typename... Args>
        static void WriteError(std::string_view format, Args... arguments)
        {
            EnableFeatures();
            std::cerr << std::vformat(format, std::make_format_args(arguments...));
        }

        /// <summary>Writes a newline to the standard error stream.</summary>
        static void WriteErrorLine();

        /// <summary>Formats and writes a string to the standard error stream with a newline appended to its end.</summary>
        /// <tparam name="Args">A parameter pack containing the arguments to be formatted.</tparam>
        /// <param name="format">The format to be used. It accepts the same specifiers as the std::format function family.</param>
        /// <param name="...">The arguments to be formatted.</param>
        template <typename... Args>
        static void WriteErrorLine(std::string_view format, Args... arguments)
        {
            WriteError(format, arguments...);
            std::cerr << std::endl;
        }
    };
}
