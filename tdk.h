/* //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
 * BSD-3-Clause License
 * Copyright (c) 2023, Sherman Rofeman <skippyr.developer@gmail.com>
 *
 * See the LICENSE file that comes in its source code for more details.
 * //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\// */

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

enum {
	tdk_ClrBlk,
	tdk_ClrRed,
	tdk_ClrGrn,
	tdk_ClrYlw,
	tdk_ClrBle,
	tdk_ClrMag,
	tdk_ClrCyn,
	tdk_ClrWht,
	tdk_ClrDft = 9
};

enum {
	tdk_EffItl = 1 << 3,
	tdk_EffUnd = 1 << 4,
	tdk_EffBlk = 1 << 5,
	tdk_EffRev = 1 << 7,
	tdk_EffCon = 1 << 8,
	tdk_EffStr = 1 << 9
};

enum {
	tdk_KeyUpArr = -22,
	tdk_KeyDnArr,
	tdk_KeyRgArr,
	tdk_KeyLfArr,
	tdk_KeyF1,
	tdk_KeyF2,
	tdk_KeyF3,
	tdk_KeyF4,
	tdk_KeyF5,
	tdk_KeyF6,
	tdk_KeyF7,
	tdk_KeyF8,
	tdk_KeyF9,
	tdk_KeyF10,
	tdk_KeyF11,
	tdk_KeyF12,
	tdk_KeyIns,
	tdk_KeyDel,
	tdk_KeyPgUp,
	tdk_KeyPgDn,
	tdk_KeyEnd,
	tdk_KeyHome,
	tdk_KeyTab = 9,
	tdk_KeyEnter,
	tdk_KeyEsc = 27,
	tdk_KeySpace = 32,
	tdk_KeyBSpace = 127
};

enum {
	tdk_LumDft,
	tdk_LumBld = 1,
	tdk_LumDim
};

enum {
	tdk_LyrFg = 3,
	tdk_LyrBg
};

enum {
	tdk_ShpDft,
	tdk_ShpBBlk,
	tdk_ShpBlk,
	tdk_ShpBUnd,
	tdk_ShpUnd,
	tdk_ShpBBar,
	tdk_ShpBar
};

#ifdef __cplusplus
extern "C" {
#endif
int tdk_getcpos(int *col, int *ln);
int tdk_getkey(void);
int tdk_getwdim(int *col, int *ln);
int tdk_setcpos(int col, int ln);
void tdk_beep(void);
void tdk_clearln(void);
void tdk_setclr(int clr, int lyr);
void tdk_setcshp(int shp);
void tdk_setcvis(int isvis);
void tdk_seteff(int eff, int isenb);
void tdk_setlum(int lum);
void tdk_setwalt(int isenb);
#ifdef __cplusplus
}
#endif
#endif
