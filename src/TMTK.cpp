#include "TMTK.hpp"
#ifdef _WIN32
#include <io.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#define INPUT_STREAM 0
#define OUTPUT_STREAM 1
#define ERROR_STREAM 2
#ifdef _WIN32
#define IS_STREAM_REDIRECTED(stream) (!_isatty(stream) << stream)
#else
#define IS_STREAM_REDIRECTED(stream) (!isatty(stream) << stream)
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
    bool Terminal::s_hasInit = false;
    bool Terminal::s_hasANSICache = false;
    // NOTE: stderr is unbuffered, thus preferred by default.
    bool Terminal::s_ansiPrefersStdOut = false;

    void Terminal::Init()
    {
        if (s_hasInit)
        {
            return;
        }
        s_hasInit = true;
        s_isInputRedirected = IS_STREAM_REDIRECTED(INPUT_STREAM);
        s_isOutputRedirected = IS_STREAM_REDIRECTED(OUTPUT_STREAM);
        s_isErrorRedirected = IS_STREAM_REDIRECTED(ERROR_STREAM);
        const char* term;
        s_allowsTextStyles = std::getenv("NO_COLOR") == nullptr || ((term = std::getenv("TERM")) && std::strcmp(term, "dumb"));
#ifdef _WIN32
        if (!SetConsoleOutputCP(CP_UTF8))
        {
            throw CannotSetOutputCPException();
        }
        s_inputHandle = GetStdHandle(STD_INPUT_HANDLE);
        s_outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        s_errorHandle = GetStdHandle(STD_ERROR_HANDLE);
        if (!EnableANSIParse(s_outputHandle) && !EnableANSIParse(s_errorHandle))
        {
            throw NoANSISupportException();
        }
#endif
    }

#ifdef _WIN32
    HANDLE Terminal::GetHandle(DWORD id)
    {
        HANDLE handle = GetStdHandle(id);
        if (handle == INVALID_HANDLE_VALUE)
        {
            throw InvalidHandleValueException();
        }
        return handle;
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
            throw StreamRedirectionException();
        }
        return bufferInfo;
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

    void Terminal::SetAllowsTextStyle(bool allowsTextStyle)
    {
        Init();
        s_allowsTextStyles = allowsTextStyle;
    }

    void Terminal::SetForeground(std::uint8_t ansiColor)
    {
        Init();
        if (s_allowsTextStyles)
        {
            WriteAnsi("\x1b[38;5;{}m", ansiColor);
        }
    }

    void Terminal::SetForeground(ANSIColor color)
    {
        Init();
        if (s_allowsTextStyles)
        {
            SetForeground(static_cast<std::uint8_t>(color));
        }
    }

    void Terminal::SetForeground(RGBColor color)
    {
        Init();
        if (s_allowsTextStyles)
        {
            WriteAnsi("\x1b[38;2;{};{};{}m", color.GetRed(), color.GetGreen(), color.GetBlue());
        }
    }

    void Terminal::SetBackground(std::uint8_t ansiColor)
    {
        WriteAnsi("\x1b[48;5;{}m", ansiColor);
    }

    void Terminal::SetBackground(ANSIColor color)
    {
        SetBackground(static_cast<std::uint8_t>(color));
    }

    void Terminal::SetBackground(RGBColor color)
    {
        WriteAnsi("\x1b[48;2;{};{};{}m", color.GetRed(), color.GetGreen(), color.GetBlue());
    }

    void Terminal::SetTextStyles(int styles)
    {
        for (int offset = 0; offset < 8; ++offset)
        {
            if (styles & 1 << offset)
            {
                switch (offset)
                {
                    /*
                     * NOTE: some terminals may apply both bold and dim effects at the same time, but technically, as they refer to the same property "text brightness", that should
                     * not be allowed. For consistent behavior, it always gets reset before applied.
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
                    break;
                default:;
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
        WriteAnsi("\x1b[39;49m");
    }

    void Terminal::ResetTextStyles()
    {
        WriteAnsi("\x1b[22;23;24;25;27;28;29m");
    }

    void Terminal::OpenAlternateScreen()
    {
        WriteAnsi("\x1b[?1049h\x1b[2J\x1b[1;1H");
    }

    void Terminal::CloseAlternateScreen()
    {
        WriteAnsi("\x1b[?1049l");
    }

    void Terminal::GetDimensions(std::optional<std::reference_wrapper<std::uint16_t>> width, std::optional<std::reference_wrapper<std::uint16_t>> height)
    {
        Init();
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo = GetScreenBufferInfo();
        if (width)
        {
            (*width).get() = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
        }
        if (height)
        {
            (*height).get() = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;
        }

#else
        winsize windowSize;
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &windowSize) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) && ioctl(STDERR_FILENO, TIOCGWINSZ, &windowSize))
        {
            throw StreamRedirectionException();
        }
        if (width)
        {
            (*width).get() = windowSize.ws_col;
        }
        if (height)
        {
            (*height).get() = windowSize.ws_row;
        }
#endif
    }

    std::uint16_t Terminal::GetWidth()
    {
        std::uint16_t width;
        GetDimensions(width, std::nullopt);
        return width;
    }

    std::uint16_t Terminal::GetHeight()
    {
        std::uint16_t height;
        GetDimensions(std::nullopt, height);
        return height;
    }

    std::uint32_t Terminal::GetArea()
    {
        std::uint16_t width;
        std::uint16_t height;
        GetDimensions(width, height);
        return width * height;
    }

    void Terminal::SetCursorCoordinate(std::uint16_t column, std::uint16_t row)
    {
        std::uint16_t columns;
        std::uint16_t rows;
        GetDimensions(columns, rows);
        if (column >= columns || row >= rows)
        {
            throw OutOfRangeException();
        }
        try
        {
            WriteAnsi("\x1b[{};{}H",  row + 1, column + 1);
        }
        catch (...)
        {
        }
    }

    void Terminal::SetCursorCoordinate(const Coordinate& coordinate)
    {
        SetCursorCoordinate(coordinate.GetColumn(), coordinate.GetRow());
    }


    void Terminal::SetCursorVisible(bool isVisible)
    {
        WriteAnsi("\x1b[?25{}", isVisible ? 'h' : 'l');
    }

    void Terminal::SetCursorStyle(CursorStyle style)
    {
        WriteAnsi("\x1b[{} q", static_cast<std::uint8_t>(style));
    }

    void Terminal::ResetCursorStyle()
    {
        WriteAnsi("\x1b[0 q");
    }

    void Terminal::RingBell()
    {
        WriteAnsi("\7");
    }

    void Terminal::Clear()
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
