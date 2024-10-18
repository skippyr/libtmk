#include "Tmk.hpp"
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
    Terminal::StreamRedirectionCache::StreamRedirectionCache()
        : m_isInputRedirected(false), m_isOutputRedirected(false), m_isErrorRedirected(false)
    {
    }

    Terminal::StreamRedirectionCache::StreamRedirectionCache(bool isInputRedirected, bool isOutputRedirected, bool isErrorRedirected)
        : m_isInputRedirected(isInputRedirected), m_isOutputRedirected(isOutputRedirected), m_isErrorRedirected(isErrorRedirected)
    {
    }

    bool Terminal::StreamRedirectionCache::IsRedirected(int fileNo)
    {
        return !fileNo ? m_isInputRedirected : fileNo == 1 ? m_isOutputRedirected : m_isErrorRedirected;
    }

    bool Terminal::Driver::m_hasEnabledFeatures = false;
    Terminal::StreamRedirectionCache Terminal::Driver::m_redirectionCache;

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
    void Terminal::Driver::SetUtf8Encoding()
    {
        SetConsoleOutputCP(CP_UTF8);
    }

    void Terminal::Driver::EnableVirtualTerminalProcessing()
    {
        SetConsoleOutputCP(CP_UTF8);
        HANDLE handle;
        DWORD mode;
        (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) || GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
            SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif

    void Terminal::Driver::EnableFeatures()
    {
        if (m_hasEnabledFeatures)
        {
            return;
        }
        m_hasEnabledFeatures = true;
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        SetUtf8Encoding();
        EnableVirtualTerminalProcessing();
#endif
        m_redirectionCache = StreamRedirectionCache(!TMK_ISATTY(0), !TMK_ISATTY(1), !TMK_ISATTY(2));
    }

    Terminal::StreamRedirectionCache& Terminal::Driver::GetStreamRedirectionCache()
    {
        return m_redirectionCache;
    }

    void Terminal::Font::SetWeight(FontWeight weight)
    {
        Driver::SendAnsiSequence("\x1b[22;{}m", (int)weight);
    }

    void Terminal::Font::ResetWeight()
    {
        Driver::SendAnsiSequence("\x1b[22m");
    }
}
