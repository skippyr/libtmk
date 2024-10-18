#include "Terminal.h"

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
#include <Windows.h>
#else
#include <unistd.h>
#endif

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
#define TMK_ISATTY(fileNo) _isatty(fileNo)
#else
#define TMK_ISATTY(fileNo) isatty(fileNo)
#endif

namespace Tmk
{
    bool Terminal::s_hasEnabledFeatures = false;
    bool Terminal::s_isInputRedirected = false;
    bool Terminal::s_isOutputRedirected = false;
    bool Terminal::s_isErrorRedirected = false;

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
    void Terminal::SetUtf8Encoding()
    {
        SetConsoleOutputCP(CP_UTF8);
    }

    void Terminal::EnableVirtualTerminalProcessing()
    {
        HANDLE handle;
        DWORD mode;
        (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) || GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
            SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif

    void Terminal::InitStreamRedirectionCache()
    {
        s_isInputRedirected = !TMK_ISATTY(0);
        s_isOutputRedirected = !TMK_ISATTY(1);
        s_isErrorRedirected = !TMK_ISATTY(2);
    }

    void Terminal::EnableFeatures()
    {
        if (s_hasEnabledFeatures)
        {
            return;
        }
        s_hasEnabledFeatures = true;
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        SetUtf8Encoding();
        EnableVirtualTerminalProcessing();
#endif
        InitStreamRedirectionCache();
    }

    bool Terminal::IsInputRedirected()
    {
        EnableFeatures();
        return s_isInputRedirected;
    }

    bool Terminal::IsOutputRedirected()
    {
        EnableFeatures();
        return s_isOutputRedirected;
    }

    bool Terminal::IsErrorRedirected()
    {
        EnableFeatures();
        return s_isErrorRedirected;
    }

    void Terminal::WriteLine()
    {
        EnableFeatures();
        std::cout << std::endl;
    }

    void Terminal::WriteErrorLine()
    {
        EnableFeatures();
        std::cerr << std::endl;
    }
}
