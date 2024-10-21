#pragma once

#include <Tmk/Exception.h>

namespace Tmk
{
    /// <summary>
    /// Represents an exception thrown when a set of terminal streams are redirected.
    /// </summary>
    class StreamRedirectionException final : public Exception
    {
    public:
        /// <summary>
        /// Creates an instance of the StreamRedirectionException class with a given message.
        /// </summary>
        /// <param name="message">A message that describes why the exception was thrown.</param>
        StreamRedirectionException(const std::string& message);
    };
}
