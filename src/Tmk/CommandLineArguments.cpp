#include "CommandLineArguments.h"

#include "EncodingConverter.h"

namespace Tmk
{
    CommandLineArguments::CommandLineArguments(int totalMainArguments, const char** mainArguments)
    {
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        const wchar_t** utf16Arguments = CommandLineToArgvW(GetCommandLineW(), &totalMainArguments);
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

    int CommandLineArguments::GetTotalArguments() const
    {
        return m_utf8Arguments.size();
    }

    const std::vector<std::string>& CommandLineArguments::GetUtf8Arguments() const
    {
        return m_utf8Arguments;
    }

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
    const std::vector<std::wstring>& CommandLineArgumnets::GetUtf16Arguments() const
    {
        return m_utf16Arguments;
    }
#endif
}
