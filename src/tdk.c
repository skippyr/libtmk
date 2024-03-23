#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

#include "tdk.h"

#define _tdk_HAS_TTY_CACHE (_tdk_g_cache & _tdk_HAS_TTY_CACHE_FLAG)
#define _tdk_HAS_TTY_CACHE_FLAG (1 << 7)
#ifdef _WIN32
#define _tdk_HIGH_SURROGATE_BEGIN 0xd800
#define _tdk_HIGH_SURROGATE_END 0xdbff
#endif
#define _tdk_IS_TTY(a_stream) (_tdk_g_cache & 1 << a_stream)
#define _tdk_PARSE_KEY(a_condition, a_key)                                                         \
    if (a_condition) {                                                                             \
        event->key = a_key;                                                                        \
        break;                                                                                     \
    }
#ifdef _WIN32
#define _tdk_TTY_CACHE(a_file, a_stream) (!!_isatty(_fileno(a_file)) << a_stream)
#else
#define _tdk_TTY_CACHE(a_file, a_stream) (!!isatty(fileno(a_file)) << a_stream)
#endif

static void _tdk_cacheTTY(void);
static int _tdk_writeANSI(const char *format, ...);

static char _tdk_g_cache = 0;

static void _tdk_cacheTTY(void)
{
    if (_tdk_HAS_TTY_CACHE) {
        return;
    }
#ifdef _WIN32
    HANDLE handle;
    DWORD mode;
    SetConsoleOutputCP(CP_UTF8);
    (GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) ||
     GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) &&
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
    _tdk_g_cache |= _tdk_TTY_CACHE(stdin, tdk_Stream_Input) |
                    _tdk_TTY_CACHE(stdout, tdk_Stream_Output) |
                    _tdk_TTY_CACHE(stderr, tdk_Stream_Error) | _tdk_HAS_TTY_CACHE_FLAG;
}

static int _tdk_writeANSI(const char *format, ...)
{
    va_list arguments;
    _tdk_cacheTTY();
    if (!_tdk_IS_TTY(tdk_Stream_Output) && !_tdk_IS_TTY(tdk_Stream_Error)) {
        return -1;
    }
    va_start(arguments, format);
    int totalBytesWritten =
        vfprintf(_tdk_IS_TTY(tdk_Stream_Output) ? stdout : stderr, format, arguments);
    va_end(arguments);
    return -(totalBytesWritten < 0);
}

void tdk_clearCursorLine(void)
{
    _tdk_writeANSI("\033[2K\033[1G");
}

void tdk_clearInputBuffer(void)
{
#ifdef _WIN32
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
    struct termios attributes;
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    if (tcgetattr(STDIN_FILENO, &attributes)) {
        return;
    }
    attributes.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    while (getchar() != EOF) {
    }
    attributes.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    fcntl(STDIN_FILENO, F_SETFL, flags);
#endif
}

int tdk_isTTY(int stream)
{
    _tdk_cacheTTY();
    return !!_tdk_IS_TTY(stream);
}

int tdk_getCursorCoordinate(struct tdk_Coordinate *coordinate)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) &&
        !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo)) {
        return -1;
    }
    coordinate->column = bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left;
    coordinate->row = bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top;
#else
    struct termios attributes;
    if (_tdk_writeANSI("\033[6n") || tcgetattr(STDIN_FILENO, &attributes)) {
        return -1;
    }
    attributes.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    int totalMatchesRead = scanf("\033[%hu;%huR", &coordinate->row, &coordinate->column);
    attributes.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    if (totalMatchesRead != 2) {
        return -1;
    }
    --coordinate->column;
    --coordinate->row;
#endif
    return 0;
}

int tdk_getWindowDimensions(struct tdk_Dimensions *dimensions)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) &&
        !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo)) {
        return -1;
    }
    dimensions->totalOfColumns = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
    dimensions->totalOfRows = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;
#else
    struct winsize windowSize;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) &&
        ioctl(STDIN_FILENO, TIOCGWINSZ, &windowSize) &&
        ioctl(STDERR_FILENO, TIOCGWINSZ, &windowSize)) {
        return -1;
    }
    dimensions->totalOfColumns = windowSize.ws_col;
    dimensions->totalOfRows = windowSize.ws_row;
#endif
    return 0;
}

int tdk_readKeyEvent(struct tdk_KeyEvent *event)
{
    _tdk_cacheTTY();
    if (!_tdk_IS_TTY(tdk_Stream_Input) || fwide(stdin, 0) > 0 ||
        (!_tdk_IS_TTY(tdk_Stream_Output) && !_tdk_IS_TTY(tdk_Stream_Error))) {
        return -1;
    }
#ifdef _WIN32
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD record;
    DWORD mode;
    DWORD totalOfEventsRead;
    int buffer;
    GetConsoleMode(handle, &mode);
    SetConsoleMode(handle, mode & ~ENABLE_PROCESSED_INPUT);
    while (1) {
        ReadConsoleInputW(handle, &record, 1, &totalOfEventsRead);
        if (record.EventType != KEY_EVENT || !record.Event.KeyEvent.bKeyDown ||
            record.Event.KeyEvent.wVirtualKeyCode == VK_CONTROL ||
            record.Event.KeyEvent.wVirtualKeyCode == VK_SHIFT ||
            record.Event.KeyEvent.wVirtualKeyCode == VK_MENU ||
            record.Event.KeyEvent.wVirtualKeyCode == VK_CAPITAL ||
            record.Event.KeyEvent.wVirtualKeyCode == VK_NUMLOCK ||
            record.Event.KeyEvent.wVirtualKeyCode == VK_SCROLL) {
            continue;
        }
        event->key = 0;
        if ((buffer = record.Event.KeyEvent.uChar.UnicodeChar)) {
            if (buffer <= 26 && buffer != tdk_Key_Tab && buffer != tdk_Key_Enter) {
                event->key = buffer + 96;
            } else if (buffer >= _tdk_HIGH_SURROGATE_BEGIN && buffer <= _tdk_HIGH_SURROGATE_END) {
                ReadConsoleInputW(handle, &record, 1, &totalOfEventsRead);
                ReadConsoleInputW(handle, &record, 1, &totalOfEventsRead);
                *((short *)&buffer + 1) = record.Event.KeyEvent.uChar.UnicodeChar;
                WideCharToMultiByte(CP_UTF8, 0, (wchar_t *)&buffer, 2, (char *)&event->key, 4, NULL,
                                    NULL);
            } else {
                WideCharToMultiByte(CP_UTF8, 0, (wchar_t *)&buffer, 1, (char *)&event->key, 4, NULL,
                                    NULL);
            }
            event->modifiers = !!(record.Event.KeyEvent.dwControlKeyState &
                                  (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) |
                               !!(record.Event.KeyEvent.dwControlKeyState &
                                  (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
                                   << 1;
            break;
        } else if (record.Event.KeyEvent.dwControlKeyState &
                   (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED | LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED |
                    SHIFT_PRESSED)) {
            continue;
        }
        event->modifiers = 0;
        _tdk_PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_LEFT &&
                           record.Event.KeyEvent.wVirtualKeyCode <= VK_DOWN,
                       record.Event.KeyEvent.wVirtualKeyCode - VK_LEFT + tdk_Key_LeftArrow);
        _tdk_PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_PRIOR &&
                           record.Event.KeyEvent.wVirtualKeyCode <= VK_HOME,
                       record.Event.KeyEvent.wVirtualKeyCode - VK_PRIOR + tdk_Key_PageUp);
        _tdk_PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_INSERT &&
                           record.Event.KeyEvent.wVirtualKeyCode <= VK_DELETE,
                       record.Event.KeyEvent.wVirtualKeyCode - VK_INSERT + tdk_Key_Insert);
        _tdk_PARSE_KEY(record.Event.KeyEvent.wVirtualKeyCode >= VK_F1 &&
                           record.Event.KeyEvent.wVirtualKeyCode <= VK_F12,
                       record.Event.KeyEvent.wVirtualKeyCode - VK_F1 + tdk_Key_F1);
    }
    SetConsoleMode(handle, mode);
#else
    struct termios attributes;
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    tcgetattr(STDIN_FILENO, &attributes);
    attributes.c_lflag &= ~(ICANON | ECHO | ISIG);
    attributes.c_iflag &= ~IXON;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    while (1) {
        char buffer[5] = {0, 0, 0, 0, 0};
        *buffer = getchar();
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
        if (*buffer == 27 && (buffer[1] = getchar()) == 79 || buffer[1] == 91) {
            for (size_t index = 2; index < 5; ++index) {
                buffer[index] = getchar();
            }
            while (getchar() != EOF) {
            }
            event->modifiers = 0;
            _tdk_PARSE_KEY(buffer[2] >= 65 && buffer[2] <= 68, buffer[2] - 65 + tdk_Key_UpArrow);
            _tdk_PARSE_KEY(buffer[3] == 126, buffer[2] - 49 + tdk_Key_Home);
            _tdk_PARSE_KEY(buffer[3] == 104 || (buffer[1] == 91 && buffer[2] == 80),
                           !(buffer[3] == 104) + tdk_Key_Insert);
            _tdk_PARSE_KEY(buffer[2] == 70 || buffer[2] == 72,
                           buffer[2] == 72 ? tdk_Key_Home : tdk_Key_End);
            _tdk_PARSE_KEY(buffer[2] >= 80 && buffer[2] <= 83, buffer[2] - 80 + tdk_Key_F1);
            _tdk_PARSE_KEY(buffer[3] >= 65 && buffer[3] <= 69, buffer[3] - 65 + tdk_Key_F1);
            _tdk_PARSE_KEY(buffer[4] == 126,
                           buffer[3] == 53                      ? tdk_Key_F5
                           : buffer[3] >= 55 && buffer[3] <= 57 ? buffer[3] - 55 + tdk_Key_F6
                           : buffer[3] == 48 || buffer[3] == 49 ? buffer[3] - 48 + tdk_Key_F9
                                                                : buffer[3] - 51 + tdk_Key_F11);
            fcntl(STDIN_FILENO, F_SETFL, flags);
            continue;
        }
        if (*buffer & 1 << 7) {
            for (size_t index = 1;
                 index < 1 + !!(*buffer & 1 << 6) + !!(*buffer & 1 << 5) + !!(*buffer & 1 << 4);
                 ++index) {
                buffer[index] = getchar();
            }
            event->key = *(int *)buffer;
            event->modifiers = 0;
        } else if ((event->key = (event->modifiers = *buffer == 27 && buffer[1] != EOF)
                                     ? buffer[1]
                                     : *buffer) >= 0 &&
                   event->key <= 26 && event->key != tdk_Key_Tab && event->key != tdk_Key_Enter) {
            event->key = !event->key ? tdk_Key_Space : event->key + 96;
            event->modifiers |= tdk_ModifierKey_Ctrl;
        }
        break;
    }
    attributes.c_lflag |= ICANON | ECHO | ISIG;
    attributes.c_iflag |= IXON;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    fcntl(STDIN_FILENO, F_SETFL, flags);
#endif
    return 0;
}

void tdk_ringBell(void)
{
    _tdk_writeANSI("\007");
}

void tdk_set256Color(int color, int layer)
{
    _tdk_writeANSI(color == tdk_Color_Default ? "\033[%d9m" : "\033[%d8;5;%dm", layer, color);
}

void tdk_setAlternateWindow(int isToEnable)
{
    _tdk_writeANSI(isToEnable ? "\033[?1049h\033[2J\033[1;1H" : "\033[?1049l");
}

int tdk_setCursorCoordinate(struct tdk_Coordinate coordinate)
{
    struct tdk_Dimensions windowDimensions;
    return -(tdk_getWindowDimensions(&windowDimensions) || ++coordinate.column <= 0 ||
             coordinate.column > windowDimensions.totalOfColumns ||
             ++coordinate.row <= 0 && coordinate.row > windowDimensions.totalOfRows ||
             _tdk_writeANSI("\033[%hu;%huH", coordinate.row, coordinate.column));
}

void tdk_setCursorShape(int shape)
{
    _tdk_writeANSI("\033[%d q", shape);
}

void tdk_setCursorVisibility(int isToShow)
{
    _tdk_writeANSI("\033[?25%c", isToShow ? 'h' : 'l');
}

void tdk_setEffect(int effect, int isToEnable)
{
    for (int ansi = 3; ansi < 10; ++ansi) {
        if (effect & 1 << ansi) {
            _tdk_writeANSI("\033[%dm", isToEnable ? ansi : ansi + 20);
        }
    }
}

void tdk_setHEXColor(int color, int layer)
{
    tdk_setRGBColor((struct tdk_RGB){color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff}, layer);
}

void tdk_setRGBColor(struct tdk_RGB color, int layer)
{
    _tdk_writeANSI("\033[%d8;2;%d;%d;%dm", layer, color.red, color.green, color.blue);
}

void tdk_setWeight(int weight)
{
    _tdk_writeANSI(weight == tdk_Weight_Default ? "\033[22m" : "\033[22;%dm", weight);
}

int tdk_write(const char *format, ...)
{
    va_list arguments;
    _tdk_cacheTTY();
    va_start(arguments, format);
    int totalBytesWritten = vprintf(format, arguments);
    va_end(arguments);
    return totalBytesWritten;
}

int tdk_writeError(const char *format, ...)
{
    va_list arguments;
    _tdk_cacheTTY();
    va_start(arguments, format);
    fflush(stdout);
    int totalBytesWritten = vfprintf(stderr, format, arguments);
    va_end(arguments);
    return totalBytesWritten;
}

int tdk_writeErrorLine(const char *format, ...)
{
    va_list arguments;
    _tdk_cacheTTY();
    va_start(arguments, format);
    fflush(stdout);
    int totalBytesWritten = vfprintf(stderr, format, arguments);
    fprintf(stderr, "\n");
    va_end(arguments);
    return totalBytesWritten < 0 ? -1 : totalBytesWritten + 1;
}

int tdk_writeLine(const char *format, ...)
{
    va_list arguments;
    _tdk_cacheTTY();
    va_start(arguments, format);
    int totalBytesWritten = vprintf(format, arguments);
    putchar('\n');
    va_end(arguments);
    return totalBytesWritten < 0 ? -1 : totalBytesWritten + 1;
}
