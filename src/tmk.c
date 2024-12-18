#include "tmk.h"
#include <stdio.h>
#if defined(_WIN32)
#include <Windows.h>
#else
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

#define HAS_CACHED_ANSI_FLAG (1 << 4)
#define HAS_INITIALIZED_FLAG (1 << 7)
#if defined(_WIN32)
#define IS_STREAM_REDIRECTED(stream_p) (!_isatty(stream_p) << stream_p)
#else
#define RAW_MODE_C_LFLAGS (ICANON | ECHO | ISIG)
#define RAW_MODE_C_IFLAGS (IXON)
#define IS_STREAM_REDIRECTED(stream_p) (!isatty(stream_p) << stream_p)
#endif

#if defined(_WIN32)
static void enableAnsiParse(void);
static char *convertUtf16ToUtf8(const wchar_t *utf16String);
#else
static void enableRawMode(void);
static void disableRawMode(void);
static void enableBlockingInput(void);
static void disableBlockingInput(void);
#endif
static void cacheStreamStates(void);
static int writeAnsi(const char *format, ...);
static void initialize(void);

static char cache_g = 0;

#if defined(_WIN32)
static void enableAnsiParse(void) {
	HANDLE handle;
	DWORD mode;
	(GetConsoleMode((handle = GetStdHandle(STD_OUTPUT_HANDLE)), &mode) || GetConsoleMode((handle = GetStdHandle(STD_ERROR_HANDLE)), &mode)) && SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

static char *convertUtf16ToUtf8(const wchar_t *utf16String) {
	int size = WideCharToMultiByte(CP_UTF8, 0, utf16String, -1, NULL, 0, NULL, NULL);
	char *buffer = malloc(size);
	WideCharToMultiByte(CP_UTF8, 0, utf16String, -1, buffer, size, NULL, NULL);
	return buffer;
}
#else
static void enableRawMode(void) {
	struct termios attributes;
	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag &= ~RAW_MODE_C_LFLAGS;
	attributes.c_iflag &= ~RAW_MODE_C_IFLAGS;
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
}

static void disableRawMode(void) {
	struct termios attributes;
	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag |= RAW_MODE_C_LFLAGS;
	attributes.c_iflag |= RAW_MODE_C_IFLAGS;
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
}

static void enableBlockingInput(void) {
	int flags = fcntl(STDIN_FILENO, F_GETFL);
	fcntl(STDIN_FILENO, F_SETFL, flags &~ O_NONBLOCK);
}

static void disableBlockingInput(void) {
	int flags = fcntl(STDIN_FILENO, F_GETFL);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}
#endif

static void cacheStreamStates(void) {
	cache_g |= IS_STREAM_REDIRECTED(tmk_Stream_Input) | IS_STREAM_REDIRECTED(tmk_Stream_Output) | IS_STREAM_REDIRECTED(tmk_Stream_Error);
}

static int writeAnsi(const char *format, ...) {
	if (!tmk_isStreamRedirected(tmk_Stream_Output)) {
		va_list arguments;
		va_start(arguments, format);
		tmk_writeArguments(format, arguments);
		va_end(arguments);
		cache_g |= HAS_CACHED_ANSI_FLAG;
	} else if (!tmk_isStreamRedirected(tmk_Stream_Error)) {
		va_list arguments;
		va_start(arguments, format);
		tmk_writeErrorArguments(format, arguments);
		va_end(arguments);
	} else {
		return -1;
	}
	return 0;
}

static void initialize(void) {
	if (cache_g & HAS_INITIALIZED_FLAG) {
		return;
	}
	cache_g |= HAS_INITIALIZED_FLAG;
#if defined(_WIN32)
	SetConsoleOutputCP(CP_UTF8);
	enableAnsiParse();
#endif
	cacheStreamStates();
}

int tmk_isStreamRedirected(int stream) {
	initialize();
	return !!(cache_g & 1 << stream);
}

void tmk_flushOutputBuffer(void) {
	fflush(stdout);
	cache_g &= ~HAS_CACHED_ANSI_FLAG;
}

void tmk_clearInputBuffer(void) {
#if defined(_WIN32)
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
	if (tmk_isStreamRedirected(tmk_Stream_Input)) {
		return;
	}
	enableRawMode();
	disableBlockingInput();
	while (getchar() != EOF) {
	}
	enableBlockingInput();
	disableRawMode();
#endif
}

void tmk_setFontAnsiColor(int color, int layer) {
	writeAnsi("\x1b[%d8;5;%dm", layer, color);
}

void tmk_setFontRgbColor(struct tmk_RgbColor color, int layer) {
	writeAnsi("\x1b[%d8;5;%d;%d;%dm", layer, color.red, color.green, color.blue);
}

void tmk_resetFontColors(void) {
	writeAnsi("\x1b[39;49m");
}

void tmk_setFontEffects(int effectsMask) {
	for (int effect = 3; effect < 10; ++effect) {
		if (effectsMask & 1 << effect && writeAnsi("\x1b[%dm", effect)) {
			return;
		}
	}
}

void tmk_resetFontEffects(void) {
	writeAnsi("\x1b[23;24;25;27;28;29m");
}

void tmk_setFontWeight(int weight) {
	writeAnsi("\x1b[22;%dm", weight);
}

void tmk_resetFontWeight(void) {
	writeAnsi("\x1b[22m");
}

void tmk_setCursorVisible(int isVisible) {
	writeAnsi("\x1b[?25%c", isVisible ? 'h' : 'l');
}

void tmk_setCursorShape(int shape, int shouldBlink) {
	writeAnsi("\x1b[%d q", shape - !!shouldBlink);
}

void tmk_resetCursorShape(void) {
	writeAnsi("\x1b[39m");
}

int tmk_getCursorCoordinate(struct tmk_Coordinate *coordinate) {
#if defined(_WIN32)
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) && !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo)) {
		return -1;
	}
	coordinate->column = bufferInfo.dwCursorPosition.X - bufferInfo.srWindow.Left;
	coordinate->row = bufferInfo.dwCursorPosition.Y - bufferInfo.srWindow.Top;
#else
	if (tmk_isStreamRedirected(tmk_Stream_Input) || (tmk_isStreamRedirected(tmk_Stream_Output) && tmk_isStreamRedirected(tmk_Stream_Error))) {
		return -1;
	}
	tmk_clearInputBuffer();
	writeAnsi("\x1b[6n");
	enableRawMode();
	scanf("\x1b[%hu;%huR", &coordinate->row, &coordinate->column);
	disableRawMode();
	--coordinate->row;
	--coordinate->column;
#endif
	return 0;
}

void tmk_setCursorCoordinate(struct tmk_Coordinate coordinate) {
	writeAnsi("\x1b[%d;%dH", coordinate.row + 1, coordinate.column + 1);
}

int tmk_getWindowDimensions(struct tmk_Dimensions *dimensions) {
#if defined(_WIN32)
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo) && !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &bufferInfo)) {
		return -1;
	}
	dimensions->totalColumns = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
	dimensions->totalRows = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;
#else
	struct winsize ioctlSize;
	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ioctlSize) && ioctl(STDOUT_FILENO, TIOCGWINSZ, &ioctlSize) && ioctl(STDERR_FILENO, TIOCGWINSZ, &ioctlSize)) {
		return -1;
	}
	dimensions->totalColumns = ioctlSize.ws_col;
	dimensions->totalRows = ioctlSize.ws_row;
#endif
	dimensions->area = dimensions->totalColumns * dimensions->totalRows;
	return 0;
}

void tmk_openAlternateWindow(void) {
	writeAnsi("\x1b[?1049h\x1b[2J\x1b[1;1H");
}

void tmk_closeAlternateWindow(void) {
	writeAnsi("\x1b[?1049l");
}

void tmk_setWindowTitle(const char *title) {
	writeAnsi("\x1b]0;%s\7", title);
}

void tmk_ringBell(void) {
	writeAnsi("\7");
}

void tmk_getCommandLineArguments(int totalMainArguments, const char **mainArguments, struct tmk_CommandLineArguments *commandLineArguments) {
#if defined(_WIN32)
	commandLineArguments->asUtf16 = CommandLineToArgvW(GetCommandLineW(), &commandLineArguments->total);
	commandLineArguments->asUtf8 = malloc(commandLineArguments->total * sizeof(void *));
	for (int offset = 0; offset < commandLineArguments->total; ++offset) {
		commandLineArguments->asUtf8[offset] = convertUtf16ToUtf8(commandLineArguments->asUtf16[offset]);
	}
#else
	commandLineArguments->total = totalMainArguments;
	commandLineArguments->asUtf8 = mainArguments;
#endif
}

void tmk_freeCommandLineArguments(struct tmk_CommandLineArguments *commandLineArguments) {
#if defined(_WIN32)
	LocalFree(commandLineArguments->asUtf16);
	for (int offset = 0; offset < commandLineArguments->total; ++offset) {
		free((void *)commandLineArguments->asUtf8[offset]);
	}
	free(commandLineArguments->asUtf8);
#endif
}

void tmk_writeArguments(const char *format, va_list arguments) {
	initialize();
	vprintf(format, arguments);
}

void tmk_writeArgumentsLine(const char *format, va_list arguments) {
	tmk_writeArguments(format, arguments);
	tmk_write("\n");
	cache_g &= ~HAS_CACHED_ANSI_FLAG;
}

void tmk_write(const char *format, ...) {
	va_list arguments;
	va_start(arguments, format);
	tmk_writeArguments(format, arguments);
	va_end(arguments);
}

void tmk_writeLine(const char *format, ...) {
	va_list arguments;
	va_start(arguments, format);
	tmk_writeArgumentsLine(format, arguments);
	va_end(arguments);
}

void tmk_writeErrorArguments(const char *format, va_list arguments) {
	initialize();
	if (cache_g & HAS_CACHED_ANSI_FLAG) {
		tmk_flushOutputBuffer();
	}
	vfprintf(stderr, format, arguments);
}

void tmk_writeErrorArgumentsLine(const char *format, va_list arguments) {
	tmk_writeErrorArguments(format, arguments);
	tmk_writeError("\n");
}

void tmk_writeError(const char *format, ...) {
	va_list arguments;
	va_start(arguments, format);
	tmk_writeErrorArguments(format, arguments);
	va_end(arguments);
}

void tmk_writeErrorLine(const char *format, ...) {
	va_list arguments;
	va_start(arguments, format);
	tmk_writeErrorArgumentsLine(format, arguments);
	va_end(arguments);
}
