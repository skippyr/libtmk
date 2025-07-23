#include "TMTK.hpp"
#ifdef _WIN32
#include <io.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace TMTK
{
    bool Terminal::s_isInputRedirected;
    bool Terminal::s_isOutputRedirected;
    bool Terminal::s_isErrorRedirected;
    bool Terminal::s_allowsTextStyles;
#ifdef _WIN32
    HANDLE Terminal::s_inputHandle;
    HANDLE Terminal::s_outputHandle;
    HANDLE Terminal::s_errorHandle;
#endif
    bool Terminal::s_hasInit{false};
    bool Terminal::s_hasANSICache{false};
    // NOTE: stderr is unbuffered, thus preferred by default.
    bool Terminal::s_ansiPrefersStdOut{false};

    void Terminal::Init()
    {
        if (s_hasInit)
        {
            return;
        }
        s_hasInit = true;
#ifdef _WIN32
        s_inputHandle = GetStdHandle(STD_INPUT_HANDLE);
        s_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        s_errorHandle = GetStdHandle(STD_ERROR_HANDLE);
        s_isInputRedirected = IsStreamRedirected(s_inputHandle);
        s_isOutputRedirected = IsStreamRedirected(s_outputHandle);
        s_isErrorRedirected = IsStreamRedirected(s_errorHandle);
        s_allowsTextStyles = std::getenv("NO_COLOR") == nullptr;
        if (!SetConsoleOutputCP(CP_UTF8))
        {
            throw CannotSetOutputCPException();
        }
        if (!EnableANSIParse(s_outputHandle) && !EnableANSIParse(s_errorHandle))
        {
            throw NoANSISupportException();
        }
#else
        s_isInputRedirected = IsStreamRedirected(STDIN_FILENO);
        s_isOutputRedirected = IsStreamRedirected(STDOUT_FILENO);
        s_isErrorRedirected = IsStreamRedirected(STDERR_FILENO);
        const char* term;
        s_allowsTextStyles = std::getenv("NO_COLOR") == nullptr || ((term = std::getenv("TERM")) && std::strcmp(term, "dumb"));
#endif
    }

#ifdef _WIN32
    HANDLE Terminal::GetHandle(DWORD id)
    {
        HANDLE handle{GetStdHandle(id)};
        if (handle == INVALID_HANDLE_VALUE)
        {
            throw InvalidHandleValueException{};
        }
        return handle;
    }

    bool Terminal::IsStreamRedirected(HANDLE handle)
    {
        if (GetFileType(handle) == FILE_TYPE_UNKNOWN && GetLastError() != NO_ERROR)
        {
            throw InvalidFileTypeException{};
        }
    }

    bool Terminal::EnableANSIParse(HANDLE handle)
    {
        DWORD mode;
        return GetConsoleMode(handle, &mode) && SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    CONSOLE_SCREEN_BUFFER_INFO Terminal::GetScreenBufferInfo()
    {
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(s_outputHandle, &bufferInfo) && !GetConsoleScreenBufferInfo(s_errorHandle, &bufferInfo))
        {
            throw StreamRedirectionException{};
        }
        return bufferInfo;
    }
#else
    bool Terminal::IsStreamRedirected(int fd)
    {
        if (isatty(fd))
        {
            return false;
        }
        if (errno != ENOTTY)
        {
            throw BadFileDescriptorException{};
        }
        return true;
    }
#endif

    [[nodiscard]]
    bool Terminal::IsInputRedirected()
    {
        Init();
        return s_isInputRedirected;
    }

    [[nodiscard]]
    bool Terminal::IsOutputRedirected()
    {
        Init();
        return s_isOutputRedirected;
    }

    [[nodiscard]]
    bool Terminal::IsErrorRedirected()
    {
        Init();
        return s_isErrorRedirected;
    }

    void Terminal::FlushOutput()
    {
        Init();
        std::cout << std::flush;
        s_hasANSICache = false;
    }

    void Terminal::FlushInput()
    {
        Init();
        if (s_isInputRedirected)
        {
            throw StreamRedirectionException{};
        }
#ifdef _WIN32
        if (!FlushConsoleInputBuffer(s_inputHandle))
        {
            throw FlushConsoleInputBufferException{};
        }
#else
        termios attributes;
        if (tcgetattr(STDIN_FILENO, &attributes))
        {
            throw TcgetattrException{};
        }
        int flags{fcntl(STDIN_FILENO, F_GETFL)};
        if (flags == -1)
        {
            throw FcntlException{};
        }
        if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK))
        {
            throw FcntlException{};
        }
        attributes.c_lflag &= ~(ECHO | ICANON);
        if (tcsetattr(STDIN_FILENO, TCSANOW, &attributes))
        {
            if (fcntl(STDIN_FILENO, F_SETFL, flags))
            {
                throw FcntlException{};
            }
            throw TcsetattrException{};
        }
        try
        {
            while (true)
            {
                if (std::cin.get() == EOF)
                {
                    break;
                }
            }
            std::cin.clear();
        }
        catch (...)
        {
            if (fcntl(STDIN_FILENO, F_SETFL, flags))
            {
                throw FcntlException{};
            }
            attributes.c_lflag |= ECHO | ICANON;
            if (tcsetattr(STDIN_FILENO, TCSANOW, &attributes))
            {
                throw TcsetattrException{};
            }
            throw;
        }
        if (fcntl(STDIN_FILENO, F_SETFL, flags))
        {
            throw FcntlException{};
        }
        attributes.c_lflag |= ECHO | ICANON;
        if (tcsetattr(STDIN_FILENO, TCSANOW, &attributes))
        {
            throw TcsetattrException{};
        }
#endif
    }

    void Terminal::SetAllowsTextStyle(bool allowsTextStyle)
    {
        Init();
        s_allowsTextStyles = allowsTextStyle;
    }

    void Terminal::SetForeground(std::uint8_t ansiColor)
    {
        Init();
        if (!s_allowsTextStyles)
        {
            return;
        }
        try
        {
            WriteAnsi("\x1b[38;5;{}m", ansiColor);
        }
        catch (InitException&)
        {
            throw;
        }
        catch (...)
        {
        }
    }

    void Terminal::SetForeground(ANSIColor color)
    {
        SetForeground(static_cast<std::uint8_t>(color));
    }

    void Terminal::SetForeground(RGBColor color)
    {
        Init();
        if (!s_allowsTextStyles)
        {
            return;
        }
        try
        {
            WriteAnsi("\x1b[38;2;{};{};{}m", color.GetRed(), color.GetGreen(), color.GetBlue());
        }
        catch (InitException&)
        {
            throw;
        }
        catch (...)
        {
        }
    }

    void Terminal::SetBackground(std::uint8_t ansiColor)
    {
        Init();
        if (!s_allowsTextStyles)
        {
            return;
        }
        try
        {
            WriteAnsi("\x1b[48;5;{}m", ansiColor);
        }
        catch (InitException&)
        {
            throw;
        }
        catch (...)
        {
        }
    }

    void Terminal::SetBackground(ANSIColor color)
    {
        SetBackground(static_cast<std::uint8_t>(color));
    }

    void Terminal::SetBackground(RGBColor color)
    {
        Init();
        if (!s_allowsTextStyles)
        {
            return;
        }
        try
        {
            WriteAnsi("\x1b[48;2;{};{};{}m", color.GetRed(), color.GetGreen(), color.GetBlue());
        }
        catch (InitException&)
        {
            throw;
        }
        catch (...)
        {
        }
    }

    void Terminal::SetTextStyles(int styles)
    {
        Init();
        if (!s_allowsTextStyles)
        {
            return;
        }
        for (int offset{}; offset < 8; ++offset)
        {
            if (styles & 1 << offset)
            {
                try
                {
                    switch (offset)
                    {
                        /*
                         * NOTE: some terminals may apply both bold and dim effects at the same time, but technically, as they refer to the same property "text brightness", that
                         * should not be allowed. For consistent behavior, it always gets reset before applied.
                         */
                    case 0: /* Bold */
                        WriteAnsi("\x1b[22;1m");
                        break;
                    case 1: /* Dim */
                        WriteAnsi("\x1b[22;2m");
                        break;
                    case 2: /* Italic */
                        WriteAnsi("\x1b[3m");
                        break;
                    case 3: /* Underline */
                        WriteAnsi("\x1b[4m");
                        break;
                    case 4: /* Strikethrough */
                        WriteAnsi("\x1b[9m");
                        break;
                    case 5: /* Blinking */
                        WriteAnsi("\x1b[5m");
                        break;
                    case 6: /* InvertedColors */
                        WriteAnsi("\x1b[7m");
                        break;
                    case 7: /* Hidden */
                        WriteAnsi("\x1b[8m");
                    default:;
                    }
                }
                catch (InitException&)
                {
                    throw;
                }
                catch (...)
                {
                    return;
                }
            }
        }
    }

    void Terminal::SetTextStyles(TextStyle style)
    {
        SetTextStyles(static_cast<int>(style));
    }

    void Terminal::ResetColors()
    {
        Init();
        if (!s_allowsTextStyles)
        {
            return;
        }
        try
        {
            WriteAnsi("\x1b[39;49m");
        }
        catch (InitException&)
        {
            throw;
        }
        catch (...)
        {
        }
    }

    void Terminal::ResetTextStyles()
    {
        Init();
        if (!s_allowsTextStyles)
        {
            return;
        }
        try
        {
            WriteAnsi("\x1b[22;23;24;25;27;28;29m");
        }
        catch (InitException&)
        {
            throw;
        }
        catch (...)
        {
        }
    }

    void Terminal::OpenAlternateScreen()
    {
        WriteAnsi("\x1b[?1049h\x1b[2J\x1b[1;1H");
    }

    void Terminal::CloseAlternateScreen()
    {
        WriteAnsi("\x1b[?1049l");
    }

    Dimensions Terminal::GetDimensions()
    {
        Init();
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo{GetScreenBufferInfo()};
        return {bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1};
#else
        winsize windowSize;
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &windowSize) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) && ioctl(STDERR_FILENO, TIOCGWINSZ, &windowSize))
        {
            throw StreamRedirectionException{};
        }
        return {windowSize.ws_col, windowSize.ws_row};
#endif
    }

    Coordinate Terminal::GetCursorCoordinate()
    {
        Init();
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo{GetScreenBufferInfo()};
        return {bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left, bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top};
#else
        if (s_isInputRedirected || (s_isOutputRedirected && s_isErrorRedirected))
        {
            throw StreamRedirectionException{};
        }
        FlushInput();
        WriteAnsi("\x1b[6n");
        termios attributes;
        if (tcgetattr(STDIN_FILENO, &attributes))
        {
            throw TcgetattrException{};
        }
        attributes.c_lflag &= ~(ECHO | ICANON);
        if (tcsetattr(STDIN_FILENO, TCSANOW, &attributes))
        {
            throw TcsetattrException{};
        }
        std::uint16_t row;
        std::uint16_t column;
        int matches = std::scanf("\x1b[%hu;%huR", &row, &column);
        attributes.c_lflag |= ECHO | ICANON;
        if (tcsetattr(STDIN_FILENO, TCSANOW, &attributes))
        {
            throw TcsetattrException{};
        }
        if (matches != 2)
        {
            FlushInput();
            throw CursorFormatException{};
        }
        return {static_cast<std::uint16_t>(column - 1), static_cast<std::uint16_t>(row - 1)};
#endif
    }

    void Terminal::SetCursorCoordinate(std::uint16_t column, std::uint16_t row)
    {
        Dimensions dimensions{GetDimensions()};
        if (column >= dimensions.GetWidth() || row >= dimensions.GetHeight())
        {
            throw OutOfRangeException{};
        }
        WriteAnsi("\x1b[{};{}H", row + 1, column + 1);
    }

    void Terminal::SetCursorCoordinate(const Coordinate& coordinate)
    {
        SetCursorCoordinate(coordinate.GetColumn(), coordinate.GetRow());
    }

    void Terminal::SetCursorVisible(bool isVisible)
    {
        try
        {
            WriteAnsi("\x1b[?25{}", isVisible ? 'h' : 'l');
        }
        catch (InitException&)
        {
            throw;
        }
        catch (...)
        {
        }
    }

    void Terminal::SetCursorStyle(CursorStyle style)
    {
        try
        {
            WriteAnsi("\x1b[{} q", static_cast<std::uint8_t>(style));
        }
        catch (InitException&)
        {
            throw;
        }
        catch (...)
        {
        }
    }

    void Terminal::ResetCursorStyle()
    {
        try
        {
            WriteAnsi("\x1b[0 q");
        }
        catch (InitException&)
        {
            throw;
        }
        catch (...)
        {
        }
    }

    void Terminal::RingBell()
    {
        WriteAnsi("\7");
    }

    void Terminal::ClearScreen()
    {
        WriteAnsi("\x1b[H\x1b[2J");
    }

    void Terminal::ClearLine()
    {
        WriteAnsi("\x1b[G\x1b[2K");
    }

    void Terminal::ClearHistory()
    {
        WriteAnsi("\x1b[H\x1b[3J");
    }

    int operator|(TextStyle style0, TextStyle style1)
    {
        return static_cast<int>(style0) | static_cast<int>(style1);
    }

    int operator|(int styles, TextStyle style)
    {
        return static_cast<int>(style) | styles;
    }

    int operator|(TextStyle style, int styles)
    {
        return static_cast<int>(style) | styles;
    }
}
