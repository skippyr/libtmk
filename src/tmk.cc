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
#define IS_STREAM_REDIRECTED(stream) (!_isatty(stream) << stream)
#else
#define IS_STREAM_REDIRECTED(stream) (!isatty(stream) << stream)
#endif

namespace tmk {
RgbColor::RgbColor(uint8_t red, uint8_t green, uint8_t blue) noexcept
    : red_(red), green_(green), blue_(blue)
{
}

uint8_t RgbColor::getRed() const noexcept { return red_; }

void RgbColor::setRed(uint8_t red) noexcept { red_ = red; }

uint8_t RgbColor::getGreen() const noexcept { return green_; }

void RgbColor::setGreen(uint8_t green) noexcept { green_ = green; }

uint8_t RgbColor::getBlue() const noexcept { return blue_; }

void RgbColor::setBlue(uint8_t blue) noexcept { blue_ = blue; }

Coordinate::Coordinate(uint16_t column, uint16_t row) noexcept
    : column_(column), row_(row)
{
}

uint16_t Coordinate::getColumn() const noexcept { return column_; }

void Coordinate::setColumn(uint16_t column) noexcept { column_ = column; }

uint16_t Coordinate::getRow() const noexcept { return row_; }

void Coordinate::setRow(uint16_t row) noexcept { row_ = row; }

Dimensions::Dimensions(uint16_t totalColumns, uint16_t totalRows) noexcept
    : totalColumns_(totalColumns), totalRows_(totalRows)
{
}

uint16_t Dimensions::getTotalColumns() const noexcept { return totalColumns_; }

uint16_t Dimensions::getTotalRows() const noexcept { return totalRows_; }

uint32_t Dimensions::getArea() const noexcept
{
    return totalColumns_ * totalRows_;
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
    : utf8String_(utf8String),
      utf16String_(Encoding::convertUtf8ToUtf16(utf8String))
{
}

MultiEncodingString::MultiEncodingString(const std::wstring &utf16String)
    : utf8String_(Encoding::convertUtf16ToUtf8(utf16String)),
      utf16String_(utf16String)
{
}
#else
MultiEncodingString::MultiEncodingString(const std::string &utf8String)
    : utf8String_(utf8String)
{
}
#endif

const std::string &MultiEncodingString::asUtf8String() const
{
    return utf8String_;
}

#if defined(_WIN32)
const std::wstring &MultiEncodingString::asUtf16String() const
{
    return utf16String_;
}
#endif

uint8_t Terminal::cache_ = 0;

#if defined(_WIN32)
void Terminal::enableAnsiParse() noexcept
{
    HANDLE handle;
    DWORD mode;
    (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
     GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#else
void Terminal::setRawInput(bool isRaw) noexcept
{
    struct termios attributes;
    tcgetattr(STDIN_FILENO, &attributes);
    attributes.c_lflag = isRaw ? attributes.c_lflag & ~(ICANON | ECHO | ISIG)
                               : attributes.c_lflag | ICANON | ECHO | ISIG;
    attributes.c_iflag =
        isRaw ? attributes.c_iflag & ~IXON : attributes.c_iflag | IXON;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
}

void Terminal::setBlockingInput(bool isBlocking) noexcept
{
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL,
          isBlocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK);
}
#endif

void Terminal::cacheStreamStates() noexcept
{
    cache_ |= IS_STREAM_REDIRECTED(0) | IS_STREAM_REDIRECTED(1) |
              IS_STREAM_REDIRECTED(2);
}

void Terminal::initialize() noexcept
{
    if (cache_ & 1 << 7) {
        return;
    }
    cache_ |= 1 << 7;
#if defined(_WIN32)
    SetConsoleOutputCP(CP_UTF8);
    enableAnsiParse();
#endif
    cacheStreamStates();
}

void Terminal::flushOutput() noexcept { std::fflush(stdout); }

void Terminal::clearInput() noexcept
{
#if defined(_WIN32)
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
    if (isInputRedirected()) {
        return;
    }
    setRawInput(true);
    setBlockingInput(false);
    while (std::getchar() != EOF) {
    }
    setBlockingInput(true);
    setRawInput(false);
#endif
}

bool Terminal::isInputRedirected() noexcept
{
    initialize();
    return cache_ & 1;
}

bool Terminal::isOutputRedirected() noexcept
{
    initialize();
    return cache_ & 1 << 1;
}

bool Terminal::isErrorRedirected() noexcept
{
    initialize();
    return cache_ & 1 << 2;
}

void Terminal::setFontColor(AnsiColor color, Layer layer) noexcept
{
    writeAnsi("\x1b[{}8;5;{}m", static_cast<int>(layer),
              static_cast<int>(color));
}

void Terminal::setFontColor(const RgbColor &color, Layer layer) noexcept
{
    writeAnsi("\x1b[{}8;2;{};{};{}m", static_cast<int>(layer), color.getRed(),
              color.getGreen(), color.getBlue());
}

void Terminal::resetFontColors() noexcept { writeAnsi("\x1b[39;49m"); }

void Terminal::setFontWeight(FontWeight weight) noexcept
{
    writeAnsi("\x1b[22;{}m", static_cast<int>(weight));
}

void Terminal::resetFontWeight() noexcept { writeAnsi("\x1b[22m"); }

void Terminal::setCursorVisible(bool isVisible) noexcept
{
    writeAnsi("\x1b[?25{}", isVisible ? 'h' : 'l');
}

void Terminal::setCursorShape(CursorShape shape, bool shouldBlink) noexcept
{
    writeAnsi("\x1b[{} q", static_cast<int>(shape) - shouldBlink);
}

void Terminal::resetCursorShape() noexcept { writeAnsi("\x1b[0 q"); }

Coordinate Terminal::getCursorCoordinate()
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
    if (isInputRedirected() || (isOutputRedirected() && isErrorRedirected())) {
        throw StreamRedirectionException();
    }
    clearInput();
    setRawInput(true);
    writeAnsi("\x1b[6n");
    uint16_t column;
    uint16_t row;
    scanf("\x1b[%hu;%huR", &row, &column);
    setRawInput(false);
    return Coordinate(column - 1, row - 1);
#endif
}

void Terminal::setCursorCoordinate(Coordinate coordinate)
{
    Dimensions windowDimensions = getWindowDimensions();
    if (coordinate.getColumn() >= windowDimensions.getTotalColumns() ||
        coordinate.getRow() >= windowDimensions.getTotalRows()) {
        throw OutOfBoundsException();
    }
    writeAnsi("\x1b[{};{}H", coordinate.getRow() + 1,
              coordinate.getColumn() + 1);
}

Coordinate Terminal::moveCursor(uint16_t steps, Direction direction)
{
    Coordinate cursorCoordinate = getCursorCoordinate();
    if (direction == Direction::Up) {
        cursorCoordinate.setRow(cursorCoordinate.getRow() - steps);
    } else if (direction == Direction::Down) {
        cursorCoordinate.setRow(cursorCoordinate.getRow() + steps);
    } else if (direction == Direction::Left) {
        cursorCoordinate.setColumn(cursorCoordinate.getColumn() - steps);
    } else {
        cursorCoordinate.setColumn(cursorCoordinate.getColumn() + steps);
    }
    setCursorCoordinate(cursorCoordinate);
    return cursorCoordinate;
}

Dimensions Terminal::getWindowDimensions()
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

void Terminal::clearWindow() noexcept { writeAnsi("\x1b[2J\x1b[1;1H"); }

void Terminal::clearLine() noexcept { writeAnsi("\x1b[2K\x1b[1G"); }

void Terminal::ringBell() noexcept { writeAnsi("\7"); }

void Terminal::openAlternateWindow() noexcept
{
    writeAnsi("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

void Terminal::closeAlternateWindow() noexcept { writeAnsi("\x1b[?1049l"); }

std::vector<MultiEncodingString>
Terminal::getArguments(int totalMainArguments, const char **mainArguments)
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

void Terminal::writeLine() noexcept
{
    initialize();
    cache_ &= ~(1 << 4);
    try {
        std::cout << std::endl;
    } catch (...) {
    }
}

void Terminal::writeErrorLine() noexcept
{
    initialize();
    if (cache_ & 1 << 4) {
        cache_ &= ~(1 << 4);
        flushOutput();
    }
    try {
        std::cerr << std::endl;
    } catch (...) {
    }
}
}
