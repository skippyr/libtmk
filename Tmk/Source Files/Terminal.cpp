#include <Tmk/Terminal.h>

#if defined(_WIN32)
#include <Windows.h>
#include <io.h>
#else
#include <sys/ioctl.h>
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
        try
        {
            Driver::WriteAnsiEscapeSequence("\x1b[{}8;5;{}m", static_cast<int>(layer), static_cast<int>(color));
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::Font::SetColor(RgbColor color, Layer layer)
    {
        try
        {
            Driver::WriteAnsiEscapeSequence("\x1b[{}8;2;{};{};{}m", static_cast<int>(layer), color.GetRed(), color.GetGreen(), color.GetBlue());
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::Font::ResetColors()
    {
        try
        {
            Driver::WriteAnsiEscapeSequence("\x1b[39;49m");
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::Font::SetWeight(FontWeight weight)
    {
        try
        {
            Driver::WriteAnsiEscapeSequence("\x1b[22;{}m", static_cast<int>(weight));
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::Font::ResetWeight()
    {
        try
        {
            Driver::WriteAnsiEscapeSequence("\x1b[22m");
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    Tmk::Dimensions Terminal::Window::GetDimensions()
    {
#if defined(_WIN32)
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) && !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
        {
            throw new StreamRedirectionException("could not get the terminal window dimensions due to the possible data source streams being redirected.");
        }
        return Dimensions(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
        struct winsize ioctlSize;
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ioctlSize) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &ioctlSize) && ioctl(STDERR_FILENO, TIOCGWINSZ, &ioctlSize))
        {
            throw new StreamRedirectionException("could not get the terminal window dimensions due to the possible data source streams being redirected.");
        }
        return Dimensions(ioctlSize.ws_col, ioctlSize.ws_row);
#endif
    }

    void Terminal::Window::OpenAlternate()
    {
        try
        {
            Driver::WriteAnsiEscapeSequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::Window::CloseAlternate()
    {
        try
        {
            Driver::WriteAnsiEscapeSequence("\x1b[?1049l");
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::Cursor::SetShape(CursorShape shape, bool isBlinking)
    {
        Driver::WriteAnsiEscapeSequence("\x1b[{} q", static_cast<int>(shape) - static_cast<int>(isBlinking));
    }

    void Terminal::Cursor::ResetShape()
    {
        Driver::WriteAnsiEscapeSequence("\x1b[0 q");
    }

    void Terminal::Cursor::SetVisible(bool isVisible)
    {
        Driver::WriteAnsiEscapeSequence("\x1b[?25{}", isVisible ? 'h' : 'l');
    }
}
