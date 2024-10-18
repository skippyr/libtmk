#pragma once

#include "System.h"

#include <string>

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
        static void WriteAnsiEscapeSequence(std::string_view format, Args... arguments);

    public:
        static bool IsInputRedirected();
        static bool IsOutputRedirected();
        static bool IsErrorRedirected();
    };
}
