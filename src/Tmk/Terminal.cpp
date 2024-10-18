#include "Terminal.h"

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
#include <Windows.h>
#include <io.h>
#else
#include <sys/ioctl.h>
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

    void Terminal::FlushOutputBuffer()
    {
        std::fflush(stdout);
    }

    void Terminal::WriteLine()
    {
        EnableFeatures();
        std::cout << std::endl;
    }

    void Terminal::WriteErrorLine()
    {
        EnableFeatures();
        FlushOutputBuffer();
        std::cerr << std::endl;
    }

    void Terminal::SetFontColor(AnsiColor color, Layer layer)
    {
        WriteAnsiEscapeSequence("\x1b[{}8;5;{}m", static_cast<int>(layer), static_cast<int>(color));
    }

    void Terminal::SetFontColor(const RgbColor& color, Layer layer)
    {
        WriteAnsiEscapeSequence("\x1b[{}8;2;{};{};{}m", static_cast<int>(layer), color.GetRed(), color.GetGreen(), color.GetBlue());
    }

    void Terminal::ResetFontColors()
    {
        WriteAnsiEscapeSequence("\x1b[39;49m");
    }

    void Terminal::SetFontWeight(FontWeight weight)
    {
        WriteAnsiEscapeSequence("\x1b[22;{}m", static_cast<int>(weight));
    }

    void Terminal::ResetFontWeight()
    {
        WriteAnsiEscapeSequence("\x1b[22m");
    }

    Dimensions Terminal::GetWindowDimensions()
    {
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) && !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
        {
            throw StreamRedirectionException();
        }
        return Dimensions(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
        struct winsize ioctlSize;
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ioctlSize) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &ioctlSize) && ioctl(STDERR_FILENO, TIOCGWINSZ, &ioctlSize))
        {
            throw StreamRedirectionException();
        }
        return Dimensions(ioctlSize.ws_col, ioctlSize.ws_row);
#endif
    }

    void Terminal::SetFontEffects(FontEffect effect)
    {
        SetFontEffects(static_cast<int>(effect));
    }

    void Terminal::SetFontEffects(int effects)
    {
        for (int offset = 3; offset < 10; ++offset)
        {
            if (effects & 1 << offset)
            {
                WriteAnsiEscapeSequence("\x1b[{}m", offset);
            }
        }
    }

    void Terminal::ResetFontEffects()
    {
        for (int offset = 23; offset < 30; ++offset)
        {
            if (offset != 26)
            {
                WriteAnsiEscapeSequence("\x1b[{}m", offset);
            }
        }
    }

    void Terminal::OpenAlternateWindow()
    {
        WriteAnsiEscapeSequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
    }

    void Terminal::CloseAlternateWindow()
    {
        WriteAnsiEscapeSequence("\x1b[?1049l");
    }

    int operator|(FontEffect effectI, FontEffect effectII)
    {
        return static_cast<int>(effectI) | static_cast<int>(effectII);
    }
}
