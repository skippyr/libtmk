#ifndef _TDK_H
#define _TDK_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

enum {
	CLN_SCR,
	CLN_LN
};

enum {
	CLR_DFT = -1,
	CLR_BLK,
	CLR_RED,
	CLR_GRN,
	CLR_YLW,
	CLR_BLE,
	CLR_MAG,
	CLR_CYN,
	CLR_WHT
};

enum {
	DRT_U = 'A',
	DRT_D,
	DRT_R,
	DRT_L
};

enum {
	EFF_ITL = 1 << 3,
	EFF_UND = 1 << 4,
	EFF_BLK = 1 << 5,
	EFF_REV = 1 << 7,
	EFF_CON = 1 << 8
};

enum {
	LUM_DFT = 22,
	LUM_STR = 1,
	LUM_WK
};

enum {
	LYR_FG = 3,
	LYR_BG
};

enum {
	SP_DFT,
	SP_BBLK,
	SP_BLK,
	SP_BUND,
	SP_UND,
	SP_BBAR,
	SP_BAR
};

int getcurpos(int *col, int *ln);
int getsize(int *col, int *ln);
int istty(FILE *fd);
void beep(void);
void clear(int cln);
void movecur(int drt, int stp);
void set256clr(int lyr, int clr);
void setaltscr(int isenb);
void setcurpos(int col, int ln);
void setcursp(int sp);
void setcurvis(int isvis);
void seteff(int eff, int isenb);
void sethexclr(int lyr, int hex);
void setlum(int lum);
void setname(char *name);
void setrgbclr(int lyr, int r, int g, int b);
#endif
