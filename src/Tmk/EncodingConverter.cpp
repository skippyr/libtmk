#include "EncodingConverter.h"

namespace Tmk
{
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
    std::string EncodingConverter::ConvertUtf16ToUtf8(const std::wstring& utf16String)
    {
        int length = WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, NULL, 0, NULL, NULL);
        std::unique_ptr<char[]> buffer = std::make_unique<char[]>(length);
        WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, buffer.get(), length, NULL, NULL);
        return buffer.get();
    }

    std::wstring EncodingConverter::ConvertUtf8ToUtf16(const std::string& utf8String)
    {
        int length = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, NULL, 0);
        std::unique_ptr<wchar_t[]> buffer = std::make_unique<wchar_t[]>(length);
        MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, buffer.get(), length);
        return buffer.get();
    }
#endif
}
