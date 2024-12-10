#include "tmk.h"
#if defined(_WIN32)
#include <Windows.h>
#include <io.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

#if defined(_WIN32)
#define IS_STREAM_REDIRECTED(a_stream) (!_isatty(a_stream) << a_stream)
#else
#define IS_STREAM_REDIRECTED(a_stream) (!isatty(a_stream) << a_stream)
#endif

namespace tmk {
RgbColor::RgbColor(uint8_t red, uint8_t green, uint8_t blue) noexcept
    : red(red), green(green), blue(blue)
{
}

uint8_t RgbColor::getRed() const noexcept { return red; }

void RgbColor::setRed(uint8_t red) noexcept { this->red = red; }

uint8_t RgbColor::getGreen() const noexcept { return green; }

void RgbColor::setGreen(uint8_t green) noexcept { this->green = green; }

uint8_t RgbColor::getBlue() const noexcept { return blue; }

void RgbColor::setBlue(uint8_t blue) noexcept { this->blue = blue; }

Coordinate::Coordinate(uint16_t column, uint16_t row) noexcept
    : column(column), row(row)
{
}

uint16_t Coordinate::getColumn() const noexcept { return column; }

void Coordinate::setColumn(uint16_t column) noexcept { this->column = column; }

uint16_t Coordinate::getRow() const noexcept { return row; }

void Coordinate::setRow(uint16_t row) noexcept { this->row = row; }

Dimensions::Dimensions(uint16_t totalColumns, uint16_t totalRows) noexcept
    : totalColumns(totalColumns), totalRows(totalRows)
{
}

uint16_t Dimensions::getTotalColumns() const noexcept { return totalColumns; }

uint16_t Dimensions::getTotalRows() const noexcept { return totalRows; }

uint32_t Dimensions::getArea() const noexcept
{
    return totalColumns * totalRows;
}

#if defined(_WIN32)
std::string Encoding::convertUtf16ToUtf8(const std::wstring &utf16String)
{
    int size = WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, nullptr,
                                   0, nullptr, nullptr);
    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(size);
    WideCharToMultiByte(CP_UTF8, 0, utf16String.c_str(), -1, buffer.get(), size,
                        nullptr, nullptr);
    return buffer.get();
}

std::wstring Encoding::convertUtf8ToUtf16(const std::string &utf8String)
{
    int size =
        MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, nullptr, 0);
    std::unique_ptr<wchar_t[]> buffer = std::make_unique<wchar_t[]>(size);
    MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, buffer.get(), size);
    return buffer.get();
}
#endif

#if defined(_WIN32)
MultiEncodingString::MultiEncodingString(const std::string &utf8String)
    : m_utf8String(utf8String),
      m_utf16String(Encoding::convertUtf8ToUtf16(utf8String))
{
}

MultiEncodingString::MultiEncodingString(const std::wstring &utf16String)
    : m_utf8String(Encoding::convertUtf16ToUtf8(utf16String)),
      m_utf16String(utf16String)
{
}
#else
MultiEncodingString::MultiEncodingString(const std::string &utf8String)
    : m_utf8String(utf8String)
{
}
#endif

const std::string &MultiEncodingString::AsUtf8String() const
{
    return m_utf8String;
}

#if defined(_WIN32)
const std::wstring &MultiEncodingString::AsUtf16String() const
{
    return m_utf16String;
}
#endif

uint8_t Terminal::m_cache = 0;

#if defined(_WIN32)
void Terminal::EnableAnsiParse() noexcept
{
    HANDLE handle;
    DWORD mode;
    (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
     GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
void Terminal::SetRawInput(bool isRaw) noexcept
{
    struct termios attributes;
    tcgetattr(STDIN_FILENO, &attributes);
    attributes.c_lflag = isRaw ? attributes.c_lflag & ~(ICANON | ECHO | ISIG)
                               : attributes.c_lflag | ICANON | ECHO | ISIG;
    attributes.c_iflag =
        isRaw ? attributes.c_iflag & ~IXON : attributes.c_iflag | IXON;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
}

void Terminal::SetBlockingInput(bool isBlocking) noexcept
{
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL,
          isBlocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
}
#endif

void Terminal::CacheStreamStates() noexcept
{
    m_cache |= IS_STREAM_REDIRECTED(0) | IS_STREAM_REDIRECTED(1) |
               IS_STREAM_REDIRECTED(2);
}

void Terminal::Initialize() noexcept
{
    if (m_cache & 1 << 7) {
        return;
    }
    m_cache |= 1 << 7;
#if defined(_WIN32)
    SetConsoleOutputCP(CP_UTF8);
    EnableAnsiParse();
#endif
    CacheStreamStates();
}

void Terminal::FlushOutput() noexcept { std::fflush(stdout); }

void Terminal::ClearInput() noexcept
{
#if defined(_WIN32)
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
    if (IsInputRedirected()) {
        return;
    }
    SetRawInput(true);
    SetBlockingInput(false);
    while (std::getchar() != EOF) {
    }
    SetBlockingInput(true);
    SetRawInput(false);
#endif
}

bool Terminal::IsInputRedirected() noexcept
{
    Initialize();
    return m_cache & 1;
}

bool Terminal::IsOutputRedirected() noexcept
{
    Initialize();
    return m_cache & 1 << 1;
}

bool Terminal::IsErrorRedirected() noexcept
{
    Initialize();
    return m_cache & 1 << 2;
}

void Terminal::SetFontColor(AnsiColor color, Layer layer) noexcept
{
    WriteAnsi("\x1b[{}8;5;{}m", static_cast<int>(layer),
              static_cast<int>(color));
}

void Terminal::SetFontColor(const RgbColor &color, Layer layer) noexcept
{
    WriteAnsi("\x1b[{}8;2;{};{};{}m", static_cast<int>(layer), color.getRed(),
              color.getGreen(), color.getBlue());
}

void Terminal::ResetFontColors() noexcept { WriteAnsi("\x1b[39;49m"); }

void Terminal::SetFontWeight(FontWeight weight) noexcept
{
    WriteAnsi("\x1b[22;{}m", static_cast<int>(weight));
}

void Terminal::ResetFontWeight() noexcept { WriteAnsi("\x1b[22m"); }

void Terminal::SetCursorVisible(bool isVisible) noexcept
{
    WriteAnsi("\x1b[?25{}", isVisible ? 'h' : 'l');
}

void Terminal::SetCursorShape(CursorShape shape, bool shouldBlink) noexcept
{
    WriteAnsi("\x1b[{} q", static_cast<int>(shape) - shouldBlink);
}

void Terminal::ResetCursorShape() noexcept { WriteAnsi("\x1b[0 q"); }

Coordinate Terminal::GetCursorCoordinate()
{
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                    &bufferInfo) &&
        !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                    &bufferInfo)) {
        throw StreamRedirectionException();
    }
    return Coordinate(bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left,
                      bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top);
#else
    if (IsInputRedirected() || (IsOutputRedirected() && IsErrorRedirected())) {
        throw StreamRedirectionException();
    }
    ClearInput();
    SetRawInput(true);
    WriteAnsi("\x1b[6n");
    uint16_t column;
    uint16_t row;
    scanf("\x1b[%hu;%huR", &row, &column);
    SetRawInput(false);
    return Coordinate(column - 1, row - 1);
#endif
}

void Terminal::SetCursorCoordinate(Coordinate coordinate)
{
    Dimensions windowDimensions = GetWindowDimensions();
    if (coordinate.getColumn() >= windowDimensions.getTotalColumns() ||
        coordinate.getRow() >= windowDimensions.getTotalRows()) {
        throw OutOfBoundsException();
    }
    WriteAnsi("\x1b[{};{}H", coordinate.getRow() + 1,
              coordinate.getColumn() + 1);
}

Coordinate Terminal::MoveCursor(uint16_t steps, Direction direction)
{
    Coordinate cursorCoordinate = GetCursorCoordinate();
    if (direction == Direction::Up) {
        cursorCoordinate.setRow(cursorCoordinate.getRow() - steps);
    } else if (direction == Direction::Down) {
        cursorCoordinate.setRow(cursorCoordinate.getRow() + steps);
    } else if (direction == Direction::Left) {
        cursorCoordinate.setColumn(cursorCoordinate.getColumn() - steps);
    } else {
        cursorCoordinate.setColumn(cursorCoordinate.getColumn() + steps);
    }
    SetCursorCoordinate(cursorCoordinate);
    return cursorCoordinate;
}

Dimensions Terminal::GetWindowDimensions()
{
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                    &bufferInfo) &&
        !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE),
                                    &bufferInfo)) {
        throw StreamRedirectionException();
    }
    return Dimensions(bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1,
                      bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);
#else
    struct winsize ioctlSize;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ioctlSize) &&
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &ioctlSize) &&
        ioctl(STDERR_FILENO, TIOCGWINSZ, &ioctlSize)) {
        throw StreamRedirectionException();
    }
    return Dimensions(ioctlSize.ws_col, ioctlSize.ws_row);
#endif
}

void Terminal::ClearWindow() noexcept { WriteAnsi("\x1b[2J\x1b[1;1H"); }

void Terminal::ClearLine() noexcept { WriteAnsi("\x1b[2K\x1b[1G"); }

void Terminal::RingBell() noexcept { WriteAnsi("\7"); }

void Terminal::OpenAlternateWindow() noexcept
{
    WriteAnsi("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

void Terminal::CloseAlternateWindow() noexcept { WriteAnsi("\x1b[?1049l"); }

std::vector<MultiEncodingString>
Terminal::GetArguments(int totalMainArguments, const char **mainArguments)
{
    std::vector<MultiEncodingString> arguments;
    if (totalMainArguments == 1) {
        return arguments;
    }
#if defined(_WIN32)
    wchar_t **utf16Arguments =
        CommandLineToArgvW(GetCommandLineW(), &totalMainArguments);
#endif
    for (int offset = 1; offset < totalMainArguments; ++offset) {
#if defined(_WIN32)
        arguments.emplace_back(utf16Arguments[offset]);
#else
        arguments.emplace_back(mainArguments[offset]);
#endif
    }
#if defined(_WIN32)
    LocalFree(utf16Arguments);
#endif
    return arguments;
}

void Terminal::WriteLine() noexcept
{
    Initialize();
    m_cache &= ~(1 << 4);
    try {
        std::cout << std::endl;
    } catch (...) {
    }
}

void Terminal::WriteErrorLine() noexcept
{
    Initialize();
    if (m_cache & 1 << 4) {
        m_cache &= ~(1 << 4);
        FlushOutput();
    }
    try {
        std::cerr << std::endl;
    } catch (...) {
    }
}
}
