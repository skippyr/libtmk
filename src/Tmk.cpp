#include "Tmk.h"
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
#include <Windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#endif

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
#define TMK_ISATTY(fileNo) _isatty(fileNo)
#else
#define TMK_ISATTY(fileNo) isatty(fileNo)
#endif

namespace Tmk
{
    RgbColor::RgbColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
        : m_red(red), m_green(green), m_blue(blue)
    {
    }

    std::uint8_t RgbColor::GetRed() const
    {
        return m_red;
    }

    void RgbColor::SetRed(std::uint8_t red)
    {
        m_red = red;
    }

    std::uint8_t RgbColor::GetGreen() const
    {
        return m_green;
    }

    void RgbColor::SetGreen(std::uint8_t green)
    {
        m_green = green;
    }

    std::uint8_t RgbColor::GetBlue() const
    {
        return m_blue;
    }

    void RgbColor::SetBlue(std::uint8_t blue)
    {
        m_blue = blue;
    }

    Dimensions::Dimensions(std::uint16_t width, std::uint16_t height)
        : m_width(width), m_height(height)
    {
    }

    std::uint16_t Dimensions::GetWidth() const
    {
        return m_width;
    }

    std::uint16_t Dimensions::GetHeight() const
    {
        return m_height;
    }

    std::uint32_t Dimensions::GetArea() const
    {
        return m_width * m_height;
    }

    Terminal::StreamRedirectionCache::StreamRedirectionCache()
        : m_isInputRedirected(false), m_isOutputRedirected(false), m_isErrorRedirected(false)
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

    void Terminal::Font::SetColor(AnsiColor color, Layer layer)
    {
        Driver::SendAnsiSequence("\x1b[{}8;5;{}m", (int)layer, (int)color);
    }

    void Terminal::Font::SetColor(RgbColor color, Layer layer)
    {
        Driver::SendAnsiSequence("\x1b[{}8;2;{};{};{}m", (int)layer, color.GetRed(), color.GetGreen(), color.GetBlue());
    }

    void Terminal::Font::ResetColors()
    {
        Driver::SendAnsiSequence("\x1b[39;49m");
    }

    Dimensions Terminal::Window::GetDimensions()
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
}
