#pragma once

#include "System.h"

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
#include <string>
#endif

namespace Tmk
{
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
    /// <summary>Represents an encoding converter used to interact with features from the Windows API.</summary>
    class EncodingConverter final
    {
    private:
        EncodingConverter() = delete;

    public:
        /// <summary>Converts an UTF-16 encoded string to UTF-8.</summary>
        /// <param name="utf16String">The UTF-16 encoded string to be converted.</param>
        /// <return>The converted string in UTF-8 encoding.</return>
        static std::string ConvertUtf16ToUtf8(const std::wstring& utf16String);
        /// <summary>Converts an UTF-8 encoded string to UTF-16.</summary>
        /// <param name="utf16String">The UTF-8 encoded string to be converted.</param>
        /// <return>The converted string in UTF-16 encoding.</return>
        static std::wstring ConvertUtf8ToUtf16(const std::string& utf8String);
    };
#endif
}
