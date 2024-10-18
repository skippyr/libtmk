#pragma once

#include "System.h"

#include <string>
#include <vector>

namespace Tmk
{
    /// <summary>Represents the command line arguments.</summary>
    class CommandLineArguments final
    {
    private:
        /// <summary>The arguments in UTF-8 encoding.</summary>
        std::vector<std::string> m_utf8Arguments;
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        /// <summary>The arguments in UTF-16 encoding.</summary>
        std::vector<std::wstring> m_utf16Arguments;
#endif

        CommandLineArguments() = delete;

    public:
        /// <summary>Creates an instance of the CommandLineArguments given the arguments from the main function.
        /// <param name="totalMainArguments">The total number of arguments received as the first argument of the main function.</param>
        /// <param name="mainArguments">The arguments received as the second argument of the main function.</param>
        CommandLineArguments(int totalMainArguments, const char** mainArguments);
        /// <summary>Gets the total number of arguments.</summary>
        /// <return>The total number of arguments</return>
        size_t GetTotalArguments() const;
        /// <summary>Gets the arguments in UTF-8 encoding.</summary>
        /// <return>The arguments in UTF-8 encoding.</return>
        const std::vector<std::string>& GetUtf8Arguments() const;
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        /// <summary>Gets the arguments in UTF-16 encoding.</summary>
        /// <return>The arguments in UTF-16 encoding.</return>
        const std::vector<std::wstring>& GetUtf16Arguments() const;
#endif
    };
}
