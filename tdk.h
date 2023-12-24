/* See LICENSE for copyright and license details. */
#ifndef _TDK_H
#define _TDK_H
#include <fcntl.h>
#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>

enum { tdk_ClrBlk, tdk_ClrRed, tdk_ClrGrn, tdk_ClrYlw, tdk_ClrBle, tdk_ClrMag,
       tdk_ClrCyn, tdk_ClrWht, tdk_ClrDft = 9 };
enum { tdk_EffItl = 1 << 3, tdk_EffUnd = 1 << 4, tdk_EffBlk = 1 << 5,
       tdk_EffRev = 1 << 7, tdk_EffCon = 1 << 8, tdk_EffStr = 1 << 9 };
enum { tdk_KeyUpArr = -22, tdk_KeyDnArr, tdk_KeyRgArr, tdk_KeyLfArr, tdk_KeyF1,
       tdk_KeyF2, tdk_KeyF3, tdk_KeyF4, tdk_KeyF5, tdk_KeyF6, tdk_KeyF7,
       tdk_KeyF8, tdk_KeyF9, tdk_KeyF10, tdk_KeyF11, tdk_KeyF12, tdk_KeyIns,
       tdk_KeyDel, tdk_KeyPgUp, tdk_KeyPgDn, tdk_KeyEnd, tdk_KeyHome,
       tdk_KeyTab = 9, tdk_KeyEnter, tdk_KeyEsc = 27, tdk_KeySpace = 32,
       tdk_KeyBSpace = 127 };
enum { tdk_LumDft, tdk_LumBld, tdk_LumDim };
enum { tdk_LyrFg = 3, tdk_LyrBg };
enum { tdk_ShpDft, tdk_ShpBBlk, tdk_ShpBlk, tdk_ShpBUnd, tdk_ShpUnd,
       tdk_ShpBBar, tdk_ShpBar };

#ifdef __cplusplus
extern "C" {
#endif
int tdk_getcpos(unsigned short int *col, unsigned short int *row);
int tdk_getkey(void);
int tdk_getwdim(unsigned short int *col, unsigned short int *row);
int tdk_setcpos(unsigned short int col, unsigned short int row);
void tdk_beep(void);
void tdk_rwipe(void);
void tdk_setclr(int lyr, int clr);
void tdk_setcshp(int shp);
void tdk_setcvis(unsigned char isvis);
void tdk_seteff(int eff, unsigned char isenb);
void tdk_setlum(int lum);
void tdk_setwalt(unsigned char isenb);
#ifdef __cplusplus
}
#endif
#endif
