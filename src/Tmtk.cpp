#include "Tmtk.hpp"
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

namespace Tmtk
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
    bool Terminal::s_hasAnsiCache = false;
    // NOTE: stderr is unbuffered, thus prefered by default.
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
        if (!EnableAnsiParse(s_outputHandle) && !EnableAnsiParse(s_errorHandle))
        {
            throw NoAnsiSupportException();
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

    bool Terminal::EnableAnsiParse(HANDLE handle)
    {
        DWORD mode;
        return GetConsoleMode(handle, &mode) && SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif

    bool Terminal::IsInputRedirected()
    {
        Init();
        return s_isInputRedirected;
    }

    bool Terminal::IsOutputRedirected()
    {
        Init();
        return s_isOutputRedirected;
    }

    bool Terminal::IsErrorRedirected()
    {
        Init();
        return s_isErrorRedirected;
    }

    void Terminal::FlushOutputBuffer()
    {
        std::cout << std::flush;
        s_hasAnsiCache = false;
    }

    void Terminal::SetFontColor(std::uint8_t ansiColor, Layer layer)
    {
        WriteAnsi("\x1b[{}8;5;{}m", static_cast<std::uint8_t>(layer), ansiColor);
    }

    void Terminal::SetFontColor(AnsiColor color, Layer layer)
    {
        SetFontColor(static_cast<std::uint8_t>(color), layer);
    }

    void Terminal::SetFontColor(RgbColor color, Layer layer)
    {
        WriteAnsi("\x1b[{}8;2;{};{};{}m", static_cast<std::uint8_t>(layer), color.GetRed(), color.GetGreen(), color.GetBlue());
    }

    void Terminal::UnsetFontColor(Layer layer)
    {
        WriteAnsi("\x1b[{}9m", static_cast<std::uint8_t>(layer));
    }

    void Terminal::UnsetFontStyles()
    {
        WriteAnsi("\x1b[0m");
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
}
