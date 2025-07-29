#ifndef _draconis_tmtk_h
#define _draconis_tmtk_h
#include <stdarg.h>

enum {
  DBLK,
  DRED,
  DGRN,
  DYLW,
  DBLE,
  DMAG,
  DCYN,
  DWHT,
  LBLK,
  LRED,
  LGRN,
  LYLW,
  LBLE,
  LMAG,
  LCYN,
  LWHT,
};

enum {
  BLD = 1,
  DIM = 1 << 1,
  ITL = 1 << 2,
  UND = 1 << 3,
  STR = 1 << 4,
  BLK = 1 << 5,
  NEG = 1 << 6,
  HID = 1 << 7,
};

struct rgb {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

#ifdef __cplusplus
extern "C" {
#endif
  int intty(void);
  int outtty(void);
  int errtty(void);
  void fout(void);
  void vwrt(const char *fmt, va_list args);
  void wrt(const char *fmt, ...);
  void vewrt(const char *fmt, va_list args);
  void ewrt(const char *fmt, ...);
  void ansifg(int clr);
  void ansibg(int clr);
  void rgbfg(struct rgb r);
  void rgbbg(struct rgb r);
  void eff(int e);
  void res(void);
#ifdef __cplusplus
}
#endif
#endif
