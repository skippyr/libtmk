#include "tmk.h"
#if defined(_WIN32)
#include <Windows.h>
#else
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
