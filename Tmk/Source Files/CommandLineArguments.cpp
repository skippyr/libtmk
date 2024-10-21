#include <Tmk/CommandLineArguments.h>

namespace Tmk
{
    CommandLineArguments::CommandLineArguments(int totalMainArguments, const char** mainArguments)
    {
#if defined(_WIN32)
        wchar_t** utf16Arguments = CommandLineToArgvW(GetCommandLineW(), &totalMainArguments);
#endif
        for (int offset = 0; offset < totalMainArguments; ++offset)
        {
#if defined(_WIN32)
            m_arguments.push_back(MultiEncodedString(utf16Arguments[offset]));
#else
            m_arguments.push_back(MultiEncodedString(mainArguments[offset]));
#endif
        }
#if defined(_WIN32)
        LocalFree(utf16Arguments);
#endif
    }

    std::size_t CommandLineArguments::GetTotalArguments() const
    {
        return m_arguments.size();
    }

    const std::vector<MultiEncodedString>& CommandLineArguments::GetArguments() const
    {
        return m_arguments;
    }
}
