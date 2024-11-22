#ifndef _tmk_H
#define _tmk_H
#include <stdarg.h>
#include <stddef.h>

enum {
  tmk_StdIn,
  tmk_StdOut,
  tmk_StdErr
};

enum {
  tmk_ShpDft,
  tmk_ShpBBlk,
  tmk_ShpBlk,
  tmk_ShpBUnd,
  tmk_ShpUnd,
  tmk_ShpBBar,
  tmk_ShpBar
};

enum {
  tmk_LyrFg = 3,
  tmk_LyrBg
};

enum {
  tmk_ClrDft = -1,
  tmk_ClrBlk,
  tmk_ClrRed,
  tmk_ClrGrn,
  tmk_ClrYlw,
  tmk_ClrBle,
  tmk_ClrMag,
  tmk_ClrCyn,
  tmk_ClrWht,
  tmk_ClrGry
};

enum {
  tmk_WgtDft,
  tmk_WgtBld,
  tmk_WgtDim
};

enum {
  tmk_DrtUp = 'A',
  tmk_DrtDn,
  tmk_DrtRg,
  tmk_DrtLf
};

enum {
  tmk_KeyUpArr = -5,
  tmk_KeyDnArr,
  tmk_KeyRgArr,
  tmk_KeyLfArr,
  tmk_KeyTab = 9,
#ifdef _WIN32
  tmk_KeyRet = 13,
#else
  tmk_KeyRet,
#endif
  tmk_KeyEsc = 27,
  tmk_KeySpc = 32,
  tmk_KeyBckspc = 127
};

enum {
  tmk_ModAlt = 1,
  tmk_ModCtrl = 1 << 1
};

struct tmk_dim {
  unsigned short cols;
  unsigned short rows;
};

struct tmk_pos {
  unsigned short col;
  unsigned short row;
};

struct tmk_key {
  int buf;
  int mods;
};

struct tmk_args {
  int total;
  const char **asutf8;
#ifdef _WIN32
  const wchar_t **asutf16;
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
char *tmk_asutf8(const wchar_t *wstr);
#endif
int tmk_istty(int std);
void tmk_flushout(void);
void tmk_clearin(void);
int tmk_getwdim(struct tmk_dim *d);
void tmk_setwalt(int isalt);
void tmk_setcvis(int isvis);
void tmk_setcshp(int shp);
int tmk_getcpos(struct tmk_pos *p);
void tmk_setcpos(struct tmk_pos p);
void tmk_mvcpos(unsigned short stp, int drt);
void tmk_setclr(int clr, int lyr);
void tmk_swpclr(int isswp);
void tmk_setwgt(int wgt);
void tmk_clearln(void);
void tmk_ringbell(void);
int tmk_readkey(int wait, int (*filt)(struct tmk_key*), struct tmk_key *key);
void tmk_getargs(int argc, const char **argv, struct tmk_args *a);
void tmk_freeargs(struct tmk_args *a);
void tmk_vwrite(const char *fmt, va_list v);
void tmk_write(const char *fmt, ...);
void tmk_vewrite(const char *fmt, va_list v);
void tmk_ewrite(const char *fmt, ...);
#ifdef __cplusplus
}
#endif
#endif
