#ifndef _TDK_H
#define _TDK_H
enum {
	TDK_CLRDFT = -1,
	TDK_CLRBLK,
	TDK_CLRRED,
	TDK_CLRGRN,
	TDK_CLRYLW,
	TDK_CLRBLE,
	TDK_CLRMAG,
	TDK_CLRCYN,
	TDK_CLRWHT,
	TDK_CLRLBLK,
	TDK_CLRLRED,
	TDK_CLRLGRN,
	TDK_CLRLYLW,
	TDK_CLRLBLE,
	TDK_CLRLMAG,
	TDK_CLRLCYN,
	TDK_CLRLWHT
};

enum {
	TDK_EFFITL = 1 << 3,
	TDK_EFFUND = 1 << 4,
	TDK_EFFBLK = 1 << 5,
	TDK_EFFREV = 1 << 7,
	TDK_EFFCON = 1 << 8,
	TDK_EFFSTR = 1 << 9
};

enum {
	TDK_KEYUARW = -23,
	TDK_KEYDARW,
	TDK_KEYRARW,
	TDK_KEYLARW,
	TDK_KEYF1,
	TDK_KEYF2,
	TDK_KEYF3,
	TDK_KEYF4,
	TDK_KEYF5,
	TDK_KEYF6,
	TDK_KEYF7,
	TDK_KEYF8,
	TDK_KEYF9,
	TDK_KEYF10,
	TDK_KEYF11,
	TDK_KEYF12,
	TDK_KEYHOME,
	TDK_KEYINS,
	TDK_KEYDEL,
	TDK_KEYEND,
	TDK_KEYPGUP,
	TDK_KEYPGDN,
	TDK_KEYTAB = 9,
	TDK_KEYRET,
	TDK_KEYESC = 27,
	TDK_KEYSPC = 32,
	TDK_KEYBSPC = 127
};

enum {
	TDK_LYRFG = 3,
	TDK_LYRBG
};

enum {
	TDK_MODALT = 1,
	TDK_MODCTRL = 1 << 1
};

enum {
	TDK_SHPDFT,
	TDK_SHPBBLK,
	TDK_SHPBLK,
	TDK_SHPBUND,
	TDK_SHPUND,
	TDK_SHPBBAR,
	TDK_SHPBAR
};

enum {
	TDK_WGTDFT,
	TDK_WGTBLD,
	TDK_WGTDIM
};

#ifdef __cplusplus
extern "C" {
#endif
void tdk_clearin(void);
void tdk_clearln(void);
int tdk_getcurpos(unsigned short *col, unsigned short *row);
int tdk_getwindim(unsigned short *cols, unsigned short *rows);
int tdk_readkey(int *key, int *mods);
void tdk_ringbell(void);
void tdk_set256clr(int clr, int lyr);
void tdk_setaltwin(int isenb);
void tdk_setcurpos(unsigned short col, unsigned short row);
void tdk_setcurshp(int shp);
void tdk_setcurvis(int isvis);
void tdk_seteff(int eff, int isenb);
void tdk_sethexclr(int clr, int lyr);
void tdk_setrgbclr(int r, int g, int b, int lyr);
void tdk_setwgt(int wgt);
#ifdef __cplusplus
}
#endif
#endif
