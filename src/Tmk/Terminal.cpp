#include "Terminal.h"

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
#include <Windows.h>
#include <io.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
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

    void Terminal::ClearInputBuffer()
    {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
        struct termios attributes;
        int flags = fcntl(STDIN_FILENO, F_GETFL);
        tcgetattr(STDIN_FILENO, &attributes);
        attributes.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        while (std::getchar() != EOF)
        {
        }
        attributes.c_lflag |= ICANON | ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
        fcntl(STDIN_FILENO, F_SETFL, flags);
#endif
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
        try
        {
            WriteAnsiEscapeSequence("\x1b[{}8;5;{}m", static_cast<int>(layer), static_cast<int>(color));
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::SetFontColor(const RgbColor& color, Layer layer)
    {
        try
        {
            WriteAnsiEscapeSequence("\x1b[{}8;2;{};{};{}m", static_cast<int>(layer), color.GetRed(), color.GetGreen(), color.GetBlue());
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::ResetFontColors()
    {
        try
        {
            WriteAnsiEscapeSequence("\x1b[39;49m");
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::SetFontWeight(FontWeight weight)
    {
        try
        {
            WriteAnsiEscapeSequence("\x1b[22;{}m", static_cast<int>(weight));
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::ResetFontWeight()
    {
        try
        {
            WriteAnsiEscapeSequence("\x1b[22m");
        }
        catch (const StreamRedirectionException&)
        {
        }
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
        try
        {
            for (int offset = 3; offset < 10; ++offset)
            {
                if (effects & 1 << offset)
                {
                    WriteAnsiEscapeSequence("\x1b[{}m", offset);
                }
            }
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::ResetFontEffects()
    {
        try
        {
            for (int offset = 23; offset < 30; ++offset)
            {
                if (offset != 26)
                {
                    WriteAnsiEscapeSequence("\x1b[{}m", offset);
                }
            }
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::SetCursorVisible(bool isVisible)
    {
        try
        {
            WriteAnsiEscapeSequence("\x1b[?25{}", isVisible ? 'h' : 'l');
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::SetCursorShape(CursorShape shape, bool isBlinking)
    {
        try
        {
            WriteAnsiEscapeSequence("\x1b[{} q", static_cast<int>(shape) - static_cast<int>(isBlinking));
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::ResetCursorShape()
    {
        try
        {
            WriteAnsiEscapeSequence("\x1b[0 q");
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    Coordinate Terminal::GetCursorCoordinate()
    {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) && !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo))
        {
            throw StreamRedirectionException();
        }
        return Coordinate(bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left, bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Right);
#else
        ClearInputBuffer();
        struct termios attributes;
        WriteAnsiEscapeSequence("\x1b[6n");
        if (tcgetattr(STDIN_FILENO, &attributes))
        {
            throw StreamRedirectionException();
        }
        unsigned short column;
        unsigned short row;
        attributes.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
        int totalMatches = std::scanf("\x1b[%hu;%huR", &row, &column);
        attributes.c_lflag |= ICANON | ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
        if (totalMatches != 2)
        {
            throw StreamRedirectionException();
        }
        return Coordinate(column - 1, row - 1);
#endif
    }

    void Terminal::ClearCursorLine()
    {
        try
        {
            WriteAnsiEscapeSequence("\x1b[2K\x1b[1G");
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::OpenAlternateWindow()
    {
        try
        {
            WriteAnsiEscapeSequence("\x1b[?1049h\x1b[2J\x1b[1;1H");
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::CloseAlternateWindow()
    {
        try
        {
            WriteAnsiEscapeSequence("\x1b[?1049l");
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    void Terminal::RingBell()
    {
        try
        {
            WriteAnsiEscapeSequence("\7");
        }
        catch (const StreamRedirectionException&)
        {
        }
    }

    int operator|(FontEffect effectI, FontEffect effectII)
    {
        return static_cast<int>(effectI) | static_cast<int>(effectII);
    }
}
