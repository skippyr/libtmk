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

enum tmk_FontEffect {
	tmk_FontEffect_Italic = 1 << 3,
	tmk_FontEffect_Underline = 1 << 4,
	tmk_FontEffect_Blinking = 1 << 5,
	tmk_FontEffect_Negative = 1 << 7,
	tmk_FontEffect_Hidden = 1 << 8,
	tmk_FontEffect_Strikethrough = 1 << 9
};

enum tmk_FontWeight {
	tmk_FontWeight_Bold = 1,
	tmk_FontWeight_Light
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
void tmk_setFontAnsiColor(int color, int layer);
void tmk_setFontRgbColor(struct tmk_RgbColor color, int layer);
void tmk_resetFontColors(void);
void tmk_setFontEffects(int effectsMask);
void tmk_resetFontEffects(void);
void tmk_setFontWeight(int weight);
void tmk_resetFontWeight(void);
int tmk_getWindowDimensions(struct tmk_Dimensions *dimensions);
void tmk_openAlternateWindow(void);
void tmk_closeAlternateWindow(void);
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
