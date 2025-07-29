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
#ifdef __cplusplus
}
#endif
#endif
