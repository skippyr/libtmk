#if !defined(_tmk_H)
#define _tmk_H
#if defined(_WIN32)
#define tmk_OS "Windows"
#define tmk_IS_WINDOWS_OS 1
#define tmk_IS_LINUX_OS 0
#define tmk_IS_MAC_OS 0
#define tmk_IS_UNKNOWN_OS 0
#if defined(_M_ARM)
#define tmk_CPU_ARCH "ARM32"
#define tmk_IS_ARM32_CPU_ARCH 1
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#elif defined(_M_ARM64)
#define tmk_CPU_ARCH "ARM64"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 1
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#elif defined(_WIN64)
#define tmk_CPU_ARCH "x86_64"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 1
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#else
#define tmk_CPU_ARCH "x86"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 1
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#endif
#else
#if defined(__linux__)
#define tmk_OS "Linux"
#define tmk_IS_WINDOWS_OS 0
#define tmk_IS_LINUX_OS 1
#define tmk_IS_MAC_OS 0
#define tmk_IS_UNKNOWN_OS 0
#elif defined(__APPLE__)
#define tmk_OS "MacOS"
#define tmk_IS_WINDOWS_OS 0
#define tmk_IS_LINUX_OS 0
#define tmk_IS_MAC_OS 1
#define tmk_IS_UNKNOWN_OS 0
#else
#define tmk_OS "Unknown Operating System"
#define tmk_IS_WINDOWS_OS 0
#define tmk_IS_LINUX_OS 0
#define tmk_IS_MAC_OS 0
#define tmk_IS_UNKNOWN_OS 1
#define tmk_CPU_ARCH "Unknown CPU Architecture"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 1
#endif
#if defined(__linux__) || defined(__APPLE__)
#if defined(__arm__)
#define tmk_CPU_ARCH "ARM32"
#define tmk_IS_ARM32_CPU_ARCH 1
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#elif defined(__aarch64__)
#define tmk_CPU_ARCH "ARM64"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 1
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#elif defined(__x86_64__)
#define tmk_CPU_ARCH "x86_64"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 1
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#elif defined(__i386__)
#define tmk_CPU_ARCH "x86"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 1
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#endif
#endif
#endif
#if tmk_IS_WINDOWS_OS
#define tmk_MIN_EXIT_CODE -2147483647
#define tmk_MAX_EXIT_CODE 2147483647
#else
#define tmk_MIN_EXIT_CODE 0
#define tmk_MAX_EXIT_CODE 255
#endif

#include <stdarg.h>

enum tmk_Stream {
  tmk_Stream_Input,
  tmk_Stream_Output,
  tmk_Stream_Error
};

enum tmk_Layer {
  tmk_Layer_Foreground = 3,
  tmk_Layer_Background
};

enum tmk_AnsiColor {
  tmk_AnsiColor_DarkBlack,
  tmk_AnsiColor_DarkRed,
  tmk_AnsiColor_DarkGreen,
  tmk_AnsiColor_DarkYellow,
  tmk_AnsiColor_DarkBlue,
  tmk_AnsiColor_DarkMagenta,
  tmk_AnsiColor_DarkCyan,
  tmk_AnsiColor_DarkWhite,
  tmk_AnsiColor_LightBlack,
  tmk_AnsiColor_LightRed,
  tmk_AnsiColor_LightGreen,
  tmk_AnsiColor_LightYellow,
  tmk_AnsiColor_LightBlue,
  tmk_AnsiColor_LightMagenta,
  tmk_AnsiColor_LightCyan,
  tmk_AnsiColor_LightWhite
};

enum tmk_Weight {
  tmk_Weight_Bold = 1,
  tmk_Weight_Dim
};

enum tmk_Effect {
  tmk_Effect_Italic = 1 << 3,
  tmk_Effect_Underline = 1 << 4,
  tmk_Effect_Blinking = 1 << 5,
  tmk_Effect_Negative = 1 << 7,
  tmk_Effect_Conceal = 1 << 8,
  tmk_Effect_Strikethrough = 1 << 9
};

enum tmk_Shape {
  tmk_Shape_BlinkingBlock = 1,
  tmk_Shape_SteadyBlock,
  tmk_Shape_BlinkingUnderline,
  tmk_Shape_SteadyUnderline,
  tmk_Shape_BlinkingBar,
  tmk_Shape_SteadyBar
};

enum tmk_Direction {
  tmk_Direction_Up = 'A',
  tmk_Direction_Down,
  tmk_Direction_Right,
  tmk_Direction_Left
};

struct tmk_RgbColor {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

struct tmk_Coordinate {
  unsigned short column;
  unsigned short row;
};

struct tmk_Dimensions {
  unsigned short totalColumns;
  unsigned short totalRows;
  unsigned int area;
};

struct tmk_Arguments {
  int total;
  const char **asUtf8;
#if tmk_IS_WINDOWS_OS
  const wchar_t **asUtf16;
#endif
};

#if defined(__cplusplus)
extern "C" {
#endif
int tmk_isStreamRedirected(int stream);
void tmk_flushOutputBuffer(void);
void tmk_clearInputBuffer(void);
void tmk_setFontAnsiColor(int color, int layer);
void tmk_setFontRgbColor(struct tmk_RgbColor color, int layer);
void tmk_resetFontColors(void);
void tmk_setFontWeight(int weight);
void tmk_resetFontWeight(void);
void tmk_setFontEffects(int effects);
void tmk_resetFontEffects(void);
void tmk_setCursorVisible(int isVisible);
void tmk_setCursorShape(int shape);
void tmk_resetCursorShape(void);
int tmk_getCursorCoordinate(struct tmk_Coordinate *coordinate);
void tmk_setCursorCoordinate(struct tmk_Coordinate coordinate);
void tmk_moveCursor(unsigned short steps, int direction);
int tmk_getWindowDimensions(struct tmk_Dimensions *dimensions);
void tmk_openAlternateWindow(void);
void tmk_closeAlternateWindow(void);
#if tmk_IS_WINDOWS_OS
char *tmk_convertUtf16ToUtf8(const wchar_t *utf16String, size_t *length);
wchar_t *tmk_convertUtf8ToUtf16(const char *utf8String, size_t *length);
#endif
void tmk_getArguments(int argc, const char **argv,
                      struct tmk_Arguments *arguments);
void tmk_freeArguments(struct tmk_Arguments *arguments);
void tmk_writeArguments(const char *format, va_list arguments);
void tmk_writeArgumentsLine(const char *format, va_list arguments);
void tmk_write(const char *format, ...);
void tmk_writeLine(const char *format, ...);
void tmk_writeErrorArguments(const char *format, va_list arguments);
void tmk_writeErrorArgumentsLine(const char *format, va_list arguments);
void tmk_writeError(const char *format, ...);
void tmk_writeErrorLine(const char *format, ...);
#if defined(__cplusplus)
}
#endif
#endif
