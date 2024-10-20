#include <Tmk/Terminal.h>

#if defined(_WIN32)
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#if defined(_WIN32)
#define TMK_ISATTY(fileNo) _isatty(fileNo)
#else
#define TMK_ISATTY(fileNo) isatty(fileNo)
#endif

namespace Tmk
{
    Terminal::StreamRedirectionCache::StreamRedirectionCache() : m_isInputRedirected(false), m_isOutputRedirected(false), m_isErrorRedirected(false)
    {
    }

    Terminal::StreamRedirectionCache::StreamRedirectionCache(bool isInputRedirected, bool isOutputRedirected, bool isErrorRedirected)
        : m_isInputRedirected(isInputRedirected), m_isOutputRedirected(isOutputRedirected), m_isErrorRedirected(isErrorRedirected)
    {
    }

    bool Terminal::StreamRedirectionCache::IsRedirected(int fileNo) const
    {
        return !fileNo ? m_isInputRedirected : fileNo == 1 ? m_isOutputRedirected : m_isErrorRedirected;
    }

    Terminal::StreamRedirectionCache Terminal::Driver::s_streamRedirectionCache;
    bool Terminal::Driver::s_hasEnabledFeatures = false;

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
    void Terminal::Driver::SetUtf8Encoding()
    {
        SetConsoleOutputCP(CP_UTF8);
    }

    void Terminal::Driver::EnableVirtualTerminalProcessing()
    {
        HANDLE handle;
        DWORD mode;
        (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) || GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
            SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif

    const Terminal::StreamRedirectionCache& Terminal::Driver::GetStreamRedirectionCache()
    {
        EnableFeatures();
        return s_streamRedirectionCache;
    }

    void Terminal::Driver::EnableFeatures()
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
        s_streamRedirectionCache = StreamRedirectionCache(!TMK_ISATTY(0), !TMK_ISATTY(1), !TMK_ISATTY(2));
    }

    void Terminal::Font::SetColor(AnsiColor color, Layer layer)
    {
        Driver::WriteAnsiEscapeSequence("\x1b[{}8;5;{}m", static_cast<int>(layer), static_cast<int>(color));
    }

    void Terminal::Font::SetColor(RgbColor color, Layer layer)
    {
        Driver::WriteAnsiEscapeSequence("\x1b[{}8;2;{};{};{}m", static_cast<int>(layer), color.GetRed(), color.GetGreen(), color.GetBlue());
    }

    void Terminal::Font::ResetColors()
    {
        Driver::WriteAnsiEscapeSequence("\x1b[39;49m");
    }
    
    void Terminal::Font::SetWeight(FontWeight weight)
    {
        Driver::WriteAnsiEscapeSequence("\x1b[22;{}m", static_cast<int>(weight));
    }
    
    void Terminal::Font::ResetWeight()
    {
        Driver::WriteAnsiEscapeSequence("\x1b[22m");
    }
}
