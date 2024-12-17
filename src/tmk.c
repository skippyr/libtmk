#include "tmk.h"
#include <stdio.h>
#if defined(_WIN32)
#include <Windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#define HAS_CACHED_ANSI_FLAG (1 << 4)
#define HAS_INITIALIZED_FLAG (1 << 7)
#if defined(_WIN32)
#define IS_STREAM_REDIRECTED(stream_p) (!_isatty(stream_p) << stream_p)
#else
#define IS_STREAM_REDIRECTED(stream_p) (!isatty(stream_p) << stream_p)
#endif

#if defined(_WIN32)
static void enableAnsiParse(void);
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

void tmk_writeArguments(const char *format, va_list arguments) {
	initialize();
	vprintf(format, arguments);
}

void tmk_writeArgumentsLine(const char *format, va_list arguments) {
	tmk_writeArguments(format, arguments);
	tmk_write("\n");
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
