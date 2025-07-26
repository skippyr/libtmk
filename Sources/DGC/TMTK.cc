/* //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\
 * TMTK.cc
 *
 * TMTK (Terminal Manipulation ToolKit) <https://github.com/skippyr/TMTK>
 * DGC :: Sherman Rofeman (skippyr) <skippyr.developer@icloud.com>
 *
 * Distributed under the BSD-3-Clause License.
 * See the LICENSE file that comes with the source code for license and copyright details.
 * //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\
 */

#include "TMTK.hh"
#include <cstring>
#ifdef _WIN32
#include <io.h>
#else
#ifdef __APPLE__
#include <crt_externs.h>
#else
#include <fstream>
#endif
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace DGC::TMTK
{
    Exception::Exception(const std::string_view& message) : m_message(message)
    {
    }

    const char* Exception::what() const noexcept
    {
        return m_message.c_str();
    }

    InitException::InitException(const std::string_view& message) : Exception(message)
    {
    }

    NotEnoughMemoryException::NotEnoughMemoryException(const std::string_view& message) : Exception(message)
    {
    }

#ifdef _WIN32
    BadEncodingException::BadEncodingException(const std::string_view& message) : Exception(message)
    {
    }
#endif

    IOException::IOException(const std::string_view& message) : Exception(message)
    {
    }

    InternalAttributesException::InternalAttributesException(const std::string_view& message) : Exception(message)
    {
    }

    FormatException::FormatException(const std::string_view& message) : Exception(message)
    {
    }

#ifdef _WIN32
    std::wstring Encoding::ConvertUTF8To16(const std::string_view& utf8String)
    {
        int size = MultiByteToWideChar(CP_UTF8, 0, utf8String.data(), -1, nullptr, 0);
        if (!size)
        {
            throw BadEncodingException("cannot convert a badly encoded UTF-8 string to UTF-16.");
        }
        std::unique_ptr<wchar_t[]> buffer = std::make_unique<wchar_t[]>(size);
        MultiByteToWideChar(CP_UTF8, 0, utf8String.data(), -1, buffer.get(), size);
        return buffer.get();
    }

    std::string Encoding::ConvertUTF16To8(const std::wstring_view& utf16String)
    {
        int size = WideCharToMultiByte(CP_UTF8, 0, utf16String.data(), -1, nullptr, 0, nullptr, nullptr);
        if (!size)
        {
            throw BadEncodingException("cannot convert a badly encoded UTF-16 string to UTF-8.");
        }
        std::unique_ptr<char[]> buffer = std::make_unique<char[]>(size);
        WideCharToMultiByte(CP_UTF8, 0, utf16String.data(), -1, buffer.get(), size, nullptr, nullptr);
        return buffer.get();
    }
#endif

#ifdef _WIN32
    UnicodeString::UnicodeString(const std::wstring_view& encodedInUTF16) : m_encodedInUTF16(encodedInUTF16), m_encodedInUTF8(Encoding::ConvertUTF16To8(encodedInUTF16))
    {
    }
#else
    UnicodeString::UnicodeString(const std::string_view& encodedInUTF8) : m_encodedInUTF8(encodedInUTF8)
    {
    }
#endif

#ifdef _WIN32
    const std::wstring& UnicodeString::EncodedInUTF16() const
    {
        return m_encodedInUTF16;
    }
#endif

    const std::string& UnicodeString::EncodedInUTF8() const
    {
        return m_encodedInUTF8;
    }

    bool Terminal::s_isInputRedirected;
    bool Terminal::s_isOutputRedirected;
    bool Terminal::s_isErrorRedirected;
    bool Terminal::s_allowsStyles;
#ifdef _WIN32
    HANDLE Terminal::s_inputHandle;
    HANDLE Terminal::s_outputHandle;
    HANDLE Terminal::s_errorHandle;
#endif
    bool Terminal::s_hasInit = false;
    bool Terminal::s_hasANSICache = false;
    /* NOTE: possible ANSI cache is avoided by preferring the standard error stream. */
    bool Terminal::s_ansiPrefersStdOut = false;

    void Terminal::Init()
    {
        if (s_hasInit)
        {
            return;
        }
        s_hasInit = true;
#ifdef _WIN32
        s_inputHandle = GetHandle(STD_INPUT_HANDLE, "input");
        s_outputHandle = GetHandle(STD_OUTPUT_HANDLE, "output");
        s_errorHandle = GetHandle(STD_ERROR_HANDLE, "error");
        s_isInputRedirected = IsStreamRedirected(s_inputHandle, "input");
        s_isOutputRedirected = IsStreamRedirected(s_outputHandle, "output");
        s_isErrorRedirected = IsStreamRedirected(s_errorHandle, "error");
        /* NOTE: learn more about the NO_COLOR environment variable at: https://no-color.org. */
        s_allowsStyles = std::getenv("NO_COLOR") == nullptr;
        if (!SetConsoleOutputCP(CP_UTF8))
        {
            throw InitException("cannot set UTF-8 as the terminal output encoding.");
        }
        if (!EnableANSIParse(s_outputHandle) && !EnableANSIParse(s_errorHandle))
        {
            throw InitException("cannot enable the terminal parse of ANSI sequences.");
        }
#else
        s_isInputRedirected = IsStreamRedirected(STDIN_FILENO, "input");
        s_isOutputRedirected = IsStreamRedirected(STDOUT_FILENO, "output");
        s_isErrorRedirected = IsStreamRedirected(STDERR_FILENO, "error");
        const char* term;
        /* NOTE: learn more about the NO_COLOR environment variable at: https://no-color.org. */
        s_allowsStyles = std::getenv("NO_COLOR") == nullptr || ((term = std::getenv("TERM")) && std::strcmp(term, "dumb"));
#endif
        /* NOTE: for safety, the standard streams are marked to throw exceptions on failure. */
        auto bits = std::ios::badbit | std::ios::failbit;
        std::cin.exceptions(bits);
        std::cout.exceptions(bits);
        std::cerr.exceptions(bits);
    }

#ifdef _WIN32
    HANDLE Terminal::GetHandle(DWORD id, const char* name)
    {
        HANDLE handle = GetStdHandle(id);
        if (handle == INVALID_HANDLE_VALUE)
        {
            throw InitException(std::format("obtained an invalid handle value for the terminal {} stream.", name));
        }
        return handle;
    }

    bool Terminal::IsStreamRedirected(HANDLE handle, const char* name)
    {
        DWORD type = GetFileType(handle);
        if (type == FILE_TYPE_UNKNOWN && GetLastError() != NO_ERROR)
        {
            throw InitException(std::format("the terminal {} stream has an invalid file type.", name));
        }
        return type != FILE_TYPE_CHAR;
    }

    bool Terminal::EnableANSIParse(HANDLE handle) noexcept
    {
        DWORD mode;
        return GetConsoleMode(handle, &mode) && SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    CONSOLE_SCREEN_BUFFER_INFO Terminal::GetScreenBufferInfo()
    {
        Init();
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(s_outputHandle, &bufferInfo) && !GetConsoleScreenBufferInfo(s_errorHandle, &bufferInfo))
        {
            throw StreamRedirectionException();
        }
        return bufferInfo;
    }
#else
    bool Terminal::IsStreamRedirected(int fd, const char* stream)
    {
        if (isatty(fd))
        {
            return false;
        }
        if (errno != ENOTTY)
        {
            throw InitException(std::format("the terminal {} stream has an invalid file descriptor.", stream));
        }
        return true;
    }

    std::vector<UnicodeString> Terminal::GetArguments()
    {
        Init();
        constexpr const char* allocationFailMessage = "cannot allocate enough memory for the terminal arguments.";
#ifdef _WIN32
        int totalArguments;
        LPWSTR* systemArguments = CommandLineToArgvW(GetCommandLineW(), &totalArguments);
        if (!systemArguments)
        {
            throw NotEnoughMemoryException(allocationFailMessage);
        }
        std::vector arguments = std::vector<UnicodeString>();
        try
        {
            arguments.reserve(totalArguments);
            for (int offset = 0; offset < totalArguments; ++offset)
            {
                arguments.emplace_back(systemArguments[offset]);
            }
        }
        catch (...)
        {
            LocalFree(systemArguments);
            throw NotEnoughMemoryException(allocationFailMessage);
        }
        LocalFree(systemArguments);
        return arguments;
#elif __APPLE__
        int totalArguments = *_NSGetArgc();
        char** systemArguments = *_NSGetArgv();
        std::vector arguments = std::vector<UnicodeString>();
        try
        {
            arguments.reserve(totalArguments);
            for (int offset = 0; offset < totalArguments; ++offset)
            {
                arguments.emplace_back(systemArguments[offset]);
            }
        }
        catch (...)
        {
            throw NotEnoughMemoryException(allocationFailMessage);
        }
        return arguments;
#else
        std::fstream cmdFile = std::fsstream("/proc/self/cmdline", std::ios::in | std::ios::binary);
        if (!cmdFile)
        {
            throw IOException("cannot open the \"/proc/self/cmdline\" file to read the terminal arguments.");
        }
        std::vector lengths = std::vector<std::size_t>();
        try
        {
            lengths.reserve(10);
            char byte;
            for (int length = 0; cmdFile.get(byte);)
            {
                if (!byte)
                {
                    lengths.emplace_back(length);
                    length = 0;
                    continue;
                }
                ++length;
            }
            cmdFile.clear();
            cmdFile.seekg(0);
            std::vector arguments = std::vector<UnicodeString>();
            arguments.reserve(lengths.size());
            for (auto length : lengths)
            {

                std::string buffer = std::string(length, 0);
                for (int offset = 0; offset < length + 1; ++offset)
                {
                    cmdFile.get(byte);
                    buffer[offset] = byte;
                }
                arguments.emplace_back(buffer);
            }
        }
        catch (...)
        {
            throw NotEnoughMemoryException(allocationFailMessage);
        }
        return arguments;
#endif
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
        if (!std::cout.rdbuf())
        {
            throw IOException("the terminal output stream does not have a buffer to flush.");
        }
        constexpr const char* flushFailMessage = "cannot flush the terminal output stream.";
        std::cout.clear();
        try
        {
            std::cout << std::flush;
        }
        catch (...)
        {
            std::cout.clear();
            throw IOException(flushFailMessage);
        }
        if (std::cout.fail())
        {
            std::cout.clear();
            throw IOException(flushFailMessage);
        }
        std::cout.clear();
        s_hasANSICache = false;
    }

    void Terminal::FlushInput()
    {
        Init();
        if (s_isInputRedirected)
        {
            throw IOException("cannot flush the terminal input streams due to it being redirected.");
        }
        if (!std::cin.rdbuf())
        {
            throw IOException("the terminal input stream does not have a buffer to flush.");
        }
        constexpr const char* flushFailMessage = "cannot flush the terminal input stream.";
#ifdef _WIN32
        if (!FlushConsoleInputBuffer(s_inputHandle))
        {
            throw IOException(flushFailMessage);
        }
#else
        termios attributes;
        constexpr const char* attributesFetchFailMessage = "cannot obtain the terminal input stream attributes.";
        constexpr const char* attributesSetFailMessage = "cannot set the terminal input stream attributes.";
        if (tcgetattr(STDIN_FILENO, &attributes))
        {
            throw InternalAttributesException(attributesFetchFailMessage);
        }
        int flags = fcntl(STDIN_FILENO, F_GETFL);
        if (flags == -1)
        {
            throw InternalAttributesException(attributesFetchFailMessage);
        }
        if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK))
        {
            throw InternalAttributesException(attributesSetFailMessage);
        }
        attributes.c_lflag &= ~(ECHO | ICANON);
        if (tcsetattr(STDIN_FILENO, TCSANOW, &attributes))
        {
            if (fcntl(STDIN_FILENO, F_SETFL, flags))
            {
                throw InternalAttributesException(attributesSetFailMessage);
            }
            throw InternalAttributesException(attributesSetFailMessage);
        }
        while (true)
        {
            try
            {
                if (std::cin.get() == EOF)
                {
                    break;
                }
            }
            catch (...)
            {
                if (std::cin.eof())
                {
                    break;
                }
                std::cin.clear();
                if (fcntl(STDIN_FILENO, F_SETFL, flags))
                {
                    throw InternalAttributesException(attributesSetFailMessage);
                }
                attributes.c_lflag |= ECHO | ICANON;
                if (tcsetattr(STDIN_FILENO, TCSANOW, &attributes))
                {
                    throw InternalAttributesException(attributesSetFailMessage);
                }
                throw IOException(flushFailMessage);
            }
        }
        std::cin.clear();
        if (fcntl(STDIN_FILENO, F_SETFL, flags))
        {
            throw InternalAttributesException(attributesSetFailMessage);
        }
        attributes.c_lflag |= ECHO | ICANON;
        if (tcsetattr(STDIN_FILENO, TCSANOW, &attributes))
        {
            throw InternalAttributesException(attributesSetFailMessage);
        }
#endif
    }

    void Terminal::SetStylesUse(bool allowsTextStyle)
    {
        Init();
        s_allowsStyles = allowsTextStyle;
    }

    void Terminal::SetForeground(std::uint8_t ansiColor)
    {
        Init();
        if (!s_allowsStyles)
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
        if (!s_allowsStyles)
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
        if (!s_allowsStyles)
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
        if (!s_allowsStyles)
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
        if (!s_allowsStyles)
        {
            return;
        }
        for (int offset = 0; offset < 8; ++offset)
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
        if (!s_allowsStyles)
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
        if (!s_allowsStyles)
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
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo = GetScreenBufferInfo();
        return Dimensions(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
        winsize windowSize;
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &windowSize) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) && ioctl(STDERR_FILENO, TIOCGWINSZ, &windowSize))
        {
            throw StreamRedirectionException();
        }
        return Dimensions(windowSize.ws_col, windowSize.ws_row);
#endif
    }

    Coordinate Terminal::GetCursorCoordinate()
    {
        Init();
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo = GetScreenBufferInfo();
        return Coordinate(bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left, bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top);
#else
        if (s_isInputRedirected || (s_isOutputRedirected && s_isErrorRedirected))
        {
            throw StreamRedirectionException();
        }
        FlushInput();
        WriteAnsi("\x1b[6n");
        termios attributes;
        if (tcgetattr(STDIN_FILENO, &attributes))
        {
            throw TcgetattrException();
        }
        attributes.c_lflag &= ~(ECHO | ICANON);
        if (tcsetattr(STDIN_FILENO, TCSANOW, &attributes))
        {
            throw TcsetattrException();
        }
        std::uint16_t row;
        std::uint16_t column;
        int matches = std::scanf("\x1b[%hu;%huR", &row, &column);
        attributes.c_lflag |= ECHO | ICANON;
        if (tcsetattr(STDIN_FILENO, TCSANOW, &attributes))
        {
            throw TcsetattrException();
        }
        if (matches != 2)
        {
            FlushInput();
            throw CursorFormatException();
        }
        return Coordinate(column - 1, row - 1);
#endif
    }

    void Terminal::SetCursorCoordinate(std::uint16_t column, std::uint16_t row)
    {
        Dimensions dimensions = GetDimensions();
        if (column >= dimensions.GetWidth() || row >= dimensions.GetHeight())
        {
            throw OutOfRangeException();
        }
        WriteAnsi("\x1b[{};{}H", row + 1, column + 1);
    }

    void Terminal::SetCursorCoordinate(const Coordinate& coordinate)
    {
        SetCursorCoordinate(coordinate.GetColumn(), coordinate.GetRow());
    }

    void Terminal::MoveCursor(std::uint16_t steps, Direction direction)
    {
        Coordinate cursorCoordinate = GetCursorCoordinate();
        if ((direction == Direction::Left && cursorCoordinate.GetColumn() - steps < 0) || (direction == Direction::Top && cursorCoordinate.GetRow() - steps < 0))
        {
            throw OutOfRangeException();
        }
        if (Dimensions dimensions = GetDimensions(); (direction == Direction::Right && cursorCoordinate.GetColumn() + steps >= dimensions.GetWidth()) ||
                                                     (direction == Direction::Left && cursorCoordinate.GetRow() + steps >= dimensions.GetHeight()))
        {
            throw OutOfRangeException();
        }
        try
        {
            WriteAnsi("\x1b[{}{}", steps, static_cast<char>(direction));
        }
        catch (InitException&)
        {
            throw;
        }
        catch (...)
        {
        }
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
