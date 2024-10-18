#include "CommandLineArguments.h"
#include "EncodingConverter.h"

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
#include <Windows.h>
#endif

namespace Tmk
{
    CommandLineArguments::CommandLineArguments(int totalMainArguments, const char** mainArguments)
    {
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        wchar_t** utf16Arguments = CommandLineToArgvW(GetCommandLineW(), &totalMainArguments);
        for (int offset = 0; offset < totalMainArguments; offset++)
        {
            m_utf16Arguments.push_back(utf16Arguments[offset]);
            m_utf8Arguments.push_back(EncodingConverter::ConvertUtf16ToUtf8(m_utf16Arguments[offset]));
        }
        LocalFree(utf16Arguments);
#else
        for (int offset = 0; offset < totalMainArguments; ++offset)
        {
            m_utf8Arguments.push_back(mainArguments[offset]);
        }
#endif
    }

    size_t CommandLineArguments::GetTotalArguments() const
    {
        return m_utf8Arguments.size();
    }

    const std::vector<std::string>& CommandLineArguments::GetUtf8Arguments() const
    {
        return m_utf8Arguments;
    }

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
    const std::vector<std::wstring>& CommandLineArguments::GetUtf16Arguments() const
    {
        return m_utf16Arguments;
    }
#endif
}
