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

enum CLN
{
	CLN_SCR,
	CLN_LN
};

enum CLR
{
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

enum EFF
{
	EFF_ITL = 1 << 3,
	EFF_UND = 1 << 4,
	EFF_BLK = 1 << 5,
	EFF_REV = 1 << 7,
	EFF_CON = 1 << 8
};

enum LUM
{
	LUM_DFT = 22,
	LUM_STR = 1,
	LUM_WK
};

enum LYR
{
	LYR_FG = 3,
	LYR_BG
};

enum SP
{
	SP_DFT,
	SP_BBLK,
	SP_BLK,
	SP_BUND,
	SP_UND,
	SP_BBAR,
	SP_BAR
};

#ifdef __cplusplus
extern "C" {
#endif
int get_cur_pos(int *col, int *ln);
int get_size(int *col, int *ln);
int is_tty(FILE *fd);
void beep(void);
void clear(int cln);
void set_256_clr(int lyr, int clr);
void set_alt_scr(int is_alt);
void set_cur_pos(int col, int ln);
void set_cur_sp(int sp);
void set_cur_vis(int is_vis);
void set_eff(int eff, int is_enb);
void set_head(const char *head);
void set_hex_clr(int lyr, int hex);
void set_lum(int lum);
void set_rgb_clr(int lyr, int r, int g, int b);
#ifdef __cplusplus
}
#endif
#endif
