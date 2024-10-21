#pragma once

#include <string>

namespace Tmk
{
    /// <summary>
    /// A string encoded in UTF-8 and UTF-16 (this one only available on Windows).
    /// </summary>
    class MultiEncodedString
    {
    private:
        /// <summary>
        /// The string encoded in UTF-8.
        /// </summary>
        std::string m_utf8String;
#if defined(_WIN32)
        /// <summary>
        /// The string encoded in UTF-16.
        /// </summary>
        std::wstring m_utf16String;
#endif

#if defined(_WIN32)
        /// <summary>
        /// Converts an UTF-16 encoded string to UTF-8.
        /// </summary>
        /// <param name="utf16String">The string encoded in UTF-16.</param>
        /// <return>The string encoded in UTF-8.</return>
        static std::string ConvertUtf16ToUtf8(const std::wstring& utf16String);
        /// <summary>
        /// Converts an UTF-8 encoded string to UTF-16.
        /// </summary>
        /// <param name="utf16String">The string encoded in UTF-8.</param>
        /// <return>The string encoded in UTF-16.</return>
        static std::wstring ConvertUtf8ToUtf16(const std::string& utf8String);
#endif

    public:
        /// <summary>
        /// Creates an instance of the MultiEncodedString class from a UTF-8 string.
        /// </summary>
        /// <param name="utf8String">The string encoded in UTF-8.</param>
        MultiEncodedString(const std::string& utf8String);
#if defined(_WIN32)
        /// <summary>
        /// Creates an instance of the MultiEncodedString class from a UTF-16 string.
        /// </summary>
        /// <param name="utf8String">The string encoded in UTF-16.</param>
        MultiEncodedString(const std::wstring& utf16String);
#endif
        /// <summary>
        /// Gets the string encoded in UTF-8.
        /// </summary>
        /// <return>The string encoded in UTF-8.</return>
        const std::string& GetUtf8String() const;
#if defined(_WIN32)
        /// <summary>
        /// Gets the string encoded in UTF-16.
        /// </summary>
        /// <return>The string encoded in UTF-16.</return>
        const std::wstring& GetUtf16String() const;
#endif
    };
}
