#ifndef _draconis_tmtk_h
#define _draconis_tmtk_h
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif
  int is_in_tty(void);
  int is_out_tty(void);
  int is_err_tty(void);
  void flush_out(void);
  void out_write_args(const char *fmt, va_list args);
  void out_write(const char *fmt, ...);
  void err_write_args(const char *fmt, va_list args);
  void err_write(const char *fmt, ...);
#ifdef __cplusplus
}
#endif
#endif
