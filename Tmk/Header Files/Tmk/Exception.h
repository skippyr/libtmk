#pragma once

#include <exception>
#include <string>

namespace Tmk
{
    /// <summary>
    /// Represents a generic exception.
    /// </summary>
    class Exception : public std::exception
    {
    private:
        /// <summary>
        /// A message that describes why the exception was thrown.
        /// </summary>
        std::string m_message;

    public:
        /// <summary>
        /// Creates an instance of the Exception class with a given message.
        /// </summary>
        /// <param name="message">A message that describes why the exception was thrown.</param>
        Exception(const std::string& message);
        const char* what() const noexcept override;
    };
}
