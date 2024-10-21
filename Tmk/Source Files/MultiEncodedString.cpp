#include <Tmk/MultiEncodedString.h>
#include <memory>
#if defined(_WIN32)
#include <Windows.h>
#endif

namespace Tmk
{
#if defined(_WIN32)
    std::string MultiEncodedString::ConvertUtf16ToUtf8(const std::wstring& utf16String)
    {
        int size = WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, NULL, 0, NULL, NULL);
        std::unique_ptr<char[]> buffer = std::make_unique<char[]>(size);
        WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, buffer.get(), size, NULL, NULL);
        return buffer.get();
    }

    std::wstring MultiEncodedString::ConvertUtf8ToUtf16(const std::string& utf8String)
    {
        int size = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, NULL, 0);
        std::unique_ptr<wchar_t[]> buffer = std::make_unique<wchar_t[]>(size);
        MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, buffer.get(), size);
        return buffer.get();
    }
#endif

#if defined(_WIN32)
    MultiEncodedString::MultiEncodedString(const std::string& utf8String) : m_utf8String(utf8String), m_utf16String(ConvertUtf8ToUtf16(utf8String))
#else
    MultiEncodedString::MultiEncodedString(const std::string& utf8String) : m_utf8String(utf8String)
#endif
    {
    }

#if defined(_WIN32)
    MultiEncodedString::MultiEncodedString(const std::wstring& utf16String) : m_utf8String(ConvertUtf16ToUtf8(utf16String)), m_utf16String(utf16String)
    {
    }
#endif

    const std::string& MultiEncodedString::GetUtf8String() const
    {
        return m_utf8String;
    }

#if defined(_WIN32)
    const std::wstring& MultiEncodedString::GetUtf16String() const
    {
        return m_utf16String;
    }
#endif
}
