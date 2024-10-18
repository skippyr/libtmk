#pragma once

#include "System.h"

#include <string>
#include <vector>

namespace Tmk
{
    class CommandLineArguments final
    {
    private:
        std::vector<std::string> m_utf8Arguments;
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        std::vector<std::wstring> m_utf16Arguments;
#endif

        CommandLineArguments() = delete;

    public:
        CommandLineArguments(int totalMainArguments, const char** mainArguments);
        size_t GetTotalArguments() const;
        const std::vector<std::string>& GetUtf8Arguments() const;
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        const std::vector<std::wstring>& GetUtf16Arguments() const;
#endif
    };
}
