#if !defined(_tmk_H)
#define _tmk_H

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

enum tmk_Effect {
	tmk_Effect_Italic = 1 << 3,
	tmk_Effect_Underline = 1 << 4,
	tmk_Effect_Blinking = 1 << 5,
	tmk_Effect_Negative = 1 << 7,
	tmk_Effect_Hidden = 1 << 8,
	tmk_Effect_Strikethrough = 1 << 9
};

enum tmk_Weight {
	tmk_Weight_Bold = 1,
	tmk_Weight_Light
};

enum tmk_Shape {
	tmk_Shape_Block = 2,
	tmk_Shape_Underline = 4,
	tmk_Shape_Bar = 6
};

struct tmk_RgbColor {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

struct tmk_Dimensions {
	unsigned short totalColumns;
	unsigned short totalRows;
	unsigned int area;
};

struct tmk_Coordinate {
	unsigned short column;
	unsigned short row;
};

struct tmk_CommandLineArguments {
	int total;
	const char **asUtf8;
#if defined(_WIN32)
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
void tmk_setFontEffects(int effectsMask);
void tmk_resetFontEffects(void);
void tmk_setFontWeight(int weight);
void tmk_resetFontWeight(void);
void tmk_setCursorShape(int shape, int shouldBlink);
void tmk_resetCursorShape(void);
int tmk_getCursorCoordinate(struct tmk_Coordinate *coordinate);
void tmk_setCursorCoordinate(struct tmk_Coordinate coordinate);
int tmk_getWindowDimensions(struct tmk_Dimensions *dimensions);
void tmk_openAlternateWindow(void);
void tmk_closeAlternateWindow(void);
void tmk_setWindowTitle(const char *title);
void tmk_ringBell(void);
void tmk_getCommandLineArguments(int totalMainArguments, const char **mainArguments, struct tmk_CommandLineArguments *commandLineArguments);
void tmk_freeCommandLineArguments(struct tmk_CommandLineArguments *arguments);
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
