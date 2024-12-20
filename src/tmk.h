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
void tmk_flushOutputBuffer(void);
void tmk_clearInputBuffer(void);
#if defined(_WIN32)
char *tmk_convertUtf16ToUtf8(const wchar_t *utf16String, size_t *length);
wchar_t *tmk_convertUtf8ToUtf16(const char *utf8String, size_t *length);
#endif
#if defined(__cplusplus)
}
#endif
#endif
