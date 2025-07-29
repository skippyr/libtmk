#ifndef _draconis_tmtk_h
#define _draconis_tmtk_h
#include <stdarg.h>

enum Stream {
  Stream_Input,
  Stream_Output,
  Stream_Error
};

#ifdef __cplusplus
extern "C" {
#endif
  int isStreamRedirected(int stream);
  void flushOutput(void);
  void outputWriteArguments(const char *format, va_list arguments);
  void outputWrite(const char *format, ...);
  void errorWriteArguments(const char *format, va_list arguments);
  void errorWrite(const char *format, ...);
#ifdef __cplusplus
}
#endif
#endif
