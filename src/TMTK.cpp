#include "TMTK.hpp"
#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
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

    void Terminal::FlushOutputBuffer()
    {
        std::cout << std::flush;
        s_hasANSICache = false;
    }

    void Terminal::SetTextForeground(std::uint8_t ansiColor)
    {
        WriteAnsi("\x1b[38;5;{}m", ansiColor);
    }

    void Terminal::SetTextForeground(ANSIColor color)
    {
        SetTextForeground(static_cast<std::uint8_t>(color));
    }

    void Terminal::SetTextForeground(RGBColor color)
    {
        WriteAnsi("\x1b[38;2;{};{};{}m", color.GetRed(), color.GetGreen(), color.GetBlue());
    }

    void Terminal::SetTextBackground(std::uint8_t ansiColor)
    {
        WriteAnsi("\x1b[48;5;{}m", ansiColor);
    }

    void Terminal::SetTextBackground(ANSIColor color)
    {
        SetTextBackground(static_cast<std::uint8_t>(color));
    }

    void Terminal::SetTextBackground(RGBColor color)
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


    void Terminal::ResetTextColors()
    {
        WriteAnsi("\x1b[39;49m");
    }

    void Terminal::ResetTextStyles()
    {
        WriteAnsi("\x1b[22;23;24;25;27;28;29m");
    }

    void Terminal::OpenAlternateWindow()
    {
        WriteAnsi("\x1b[?1049h\x1b[2J\x1b[1;1H");
    }

    void Terminal::CloseAlternateWindow()
    {
        WriteAnsi("\x1b[?1049l");
    }

    void Terminal::RingBell()
    {
        WriteAnsi("\7");
    }

    int operator|(TextStyle style0, TextStyle style1)
    {
        return static_cast<int>(style0) | static_cast<int>(style1);
    }

    int operator|(int styles, TextStyle style)
    {
        return static_cast<int>(style) | styles;
    }
}
