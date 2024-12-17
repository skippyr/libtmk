#if !defined(_tmk_H)
#define _tmk_H

#include <stdarg.h>

enum tmk_Stream {
	tmk_Stream_Input,
	tmk_Stream_Output,
	tmk_Stream_Error
};

#if defined(__cplusplus)
extern "C" {
#endif
int tmk_isStreamRedirected(int stream);
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
