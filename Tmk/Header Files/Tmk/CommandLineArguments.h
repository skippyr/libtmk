#pragma once

#include <Tmk/MultiEncodedString.h>
#include <vector>

namespace Tmk
{
    /// <summary>
    /// Represents the command line arguments.
    /// </summary>
    class CommandLineArguments
    {
    private:
        /// <summary>
        /// The arguments.
        /// </summary>
        std::vector<MultiEncodedString> m_arguments;

    public:
        /// <summary>
        /// Creates an instance of the command line arguments class from the values received as the main function parameters.
        /// </summary>
        /// <param name="totalMainArguments">The total number of arguments received as the first parameter of the main function.</param>
        /// <param name="mainArguments">The arguments received as the second parameter of the main function.</param>
        CommandLineArguments(int totalMainArguments, const char** mainArguments);
        /// <summary>
        /// Gets the total number of arguments.
        /// </summary>
        /// <returns>The total number of arguments.</returns>
        [[nodiscard]] std::size_t GetTotalArguments() const;
        /// <summary>
        /// Gets the arguments.
        /// </summary>
        /// <return>The arguments.</return>
        [[nodiscard]] const std::vector<MultiEncodedString>& GetArguments() const;
    };
}
