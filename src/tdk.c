#include <stdarg.h>
#include <stdio.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>

#include "tdk.h"

#define _TDK_ISTTY(fd_a) (_tdk_cache_g & 1 << fd_a)
#define _TDK_PARSEKEY(cond_a, key_a) \
	if (cond_a) { \
		tmpkey = key_a; \
		break; \
	}

static void _tdk_cachetty(void);
static int _tdk_writeansi(char *fmt, ...);

static int _tdk_cache_g = 0;

static void
_tdk_cachetty(void)
{
	if (!(_tdk_cache_g & 1 << 7))
		_tdk_cache_g |= isatty(0) | isatty(1) << 1 | isatty(2) << 2 | 1 << 7;
}

static int
_tdk_writeansi(char *fmt, ...)
{
	va_list args;
	int bytes;
	_tdk_cachetty();
	if (!_TDK_ISTTY(1) && !_TDK_ISTTY(2))
		return -1;
	va_start(args, fmt);
	bytes = vfprintf(_TDK_ISTTY(1) ? stdout : stderr, fmt, args);
	va_end(args);
	return -(bytes < 0);
}

void
tdk_clearin(void)
{
	struct termios t;
	int fl;
	if (tcgetattr(0, &t))
		return;
	fl = fcntl(0, F_GETFL);
	t.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &t);
	fcntl(0, F_SETFL, fl | O_NONBLOCK);
	while (getchar() != EOF);
	t.c_lflag |= ICANON | ECHO;
	tcsetattr(0, TCSANOW, &t);
	fcntl(0, F_SETFL, fl);
}

void
tdk_clearln(void)
{
	_tdk_writeansi("\033[2K\033[1G");
}

int
tdk_getcurpos(unsigned short *col, unsigned short *row)
{
	struct termios t;
	unsigned short tmpcol;
	unsigned short tmprow;
	tdk_clearin();
	if (_tdk_writeansi("\033[6n") || tcgetattr(0, &t))
		return -1;
	t.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &t);
	int mat = scanf("\033[%hu;%huR", &tmprow, &tmpcol);
	t.c_lflag |= ICANON | ECHO;
	tcsetattr(0, TCSANOW, &t);
	if (mat != 2)
		return -1;
	if (col)
		*col = tmpcol - 1;
	if (row)
		*row = tmprow - 1;
	return 0;
}

int
tdk_getwindim(unsigned short *cols, unsigned short *rows)
{
	struct winsize w;
	if (ioctl(1, TIOCGWINSZ, &w) && ioctl(0, TIOCGWINSZ, &w) &&
		ioctl(2, TIOCGWINSZ, &w))
		return -1;
	if (cols)
		*cols = w.ws_col;
	if (rows)
		*rows = w.ws_row;
	return 0;
}

int
tdk_readkey(int *key, int *mods)
{
	int tmpkey = 0;
	int tmpmods = 0;
	int fl;
	int i;
	struct termios t;
	char buf[5];
	_tdk_cachetty();
	if (!_TDK_ISTTY(0) || fwide(stdin, 0) > 0 ||
		(!_TDK_ISTTY(1) && !_TDK_ISTTY(2)))
		return -1;
	tcgetattr(0, &t);
	fl = fcntl(0, F_GETFL);
	t.c_lflag &= ~(ICANON | ECHO | ISIG);
	t.c_iflag &= ~IXON;
	tcsetattr(0, TCSANOW, &t);
	while (1) {
		*(int *)buf = 0;
		buf[4] = 0;
		*buf = getchar();
		fcntl(0, F_SETFL, fl | O_NONBLOCK);
		if (*buf == 27 && ((buf[1] = getchar()) == 79 || buf[1] == 91)) {
			for (i = 2; i < 5; ++i)
				buf[i] = getchar();
			while (getchar() != EOF);
			_TDK_PARSEKEY(buf[2] >= 65 && buf[2] <= 68,
						  buf[2] - 65 + TDK_KEYUARW);
			_TDK_PARSEKEY(buf[3] == 126, buf[2] - 49 + TDK_KEYHOME);
			_TDK_PARSEKEY(buf[3] == 104 || (buf[1] == 91 && buf[2] == 80),
						  !(buf[3] == 104) + TDK_KEYINS);
			_TDK_PARSEKEY(buf[2] == 70 || buf[2] == 72, buf[2] == 72 ?
						  TDK_KEYHOME : TDK_KEYEND);
			_TDK_PARSEKEY(buf[2] >= 80 && buf[2] <= 83,
						  buf[2] - 80 + TDK_KEYF1);
			_TDK_PARSEKEY(buf[3] >= 65 && buf[3] <= 69,
						  buf[3] - 65 + TDK_KEYF1);
			_TDK_PARSEKEY(buf[4] == 126, buf[3] == 53 ? TDK_KEYF5 :
						  buf[3] >= 55 && buf[3] <= 57 ?
						  buf[3] - 55 + TDK_KEYF6 : buf[3] == 48 ||
						  buf[3] == 49 ? buf[3] - 48 + TDK_KEYF9 :
						  buf[3] - 51 + TDK_KEYF11);
			fcntl(0, F_SETFL, fl);
			continue;
		}
		if (*buf & 1 << 7) {
			for (i = 1; i < 1 + !!(*buf & 1 << 6) + !!(*buf & 1 << 5) +
				 !!(*buf & 1 << 4); ++i)
				buf[i] = getchar();
			tmpkey = *(int *)buf;
		} else if ((tmpkey = (tmpmods = *buf == 27 && buf[1] != EOF) ?
				   buf[1] : *buf) >= 0 && tmpkey <= 26 &&
				   tmpkey != TDK_KEYTAB && tmpkey != TDK_KEYRET) {
			tmpkey = !tmpkey ? TDK_KEYSPC : tmpkey + 96;
			tmpmods |= TDK_MODCTRL;
		}
		break;
	}
	t.c_lflag |= ICANON | ECHO | ISIG;
	t.c_iflag |= IXON;
	tcsetattr(0, TCSANOW, &t);
	fcntl(STDIN_FILENO, F_SETFL, fl);
	if (key)
		*key = tmpkey;
	if (mods)
		*mods = tmpmods;
	return 0;
}

void
tdk_ringbell(void)
{
	_tdk_writeansi("\7");
}

void
tdk_set256clr(int clr, int lyr)
{
	_tdk_writeansi(clr == TDK_CLRDFT ? "\033[%d9m" : "\033[%d8;5;%dm", lyr,
				   clr);
}

void
tdk_setaltwin(int isenb)
{
	_tdk_writeansi(isenb ? "\033[?1049h\033[2J\033[1;1H" : "\033[?1049l");
}

void
tdk_setcurpos(unsigned short col, unsigned short row)
{
	_tdk_writeansi("\033[%hu;%huH", row + 1, col + 1);
}

void
tdk_setcurshp(int shp)
{
	_tdk_writeansi("\033[%d q", shp);
}

void
tdk_setcurvis(int isvis)
{
	_tdk_writeansi("\033[?25%c", isvis ? 'h' : 'l');
}

void
tdk_seteff(int eff, int isenb)
{
	int i;
	for (i = 0; i < 9; ++i)
		if (eff & 1 << i)
			_tdk_writeansi("\033[%dm", isenb ? i : i + 20);
}

void
tdk_sethexclr(int clr, int lyr)
{
	tdk_setrgbclr(clr >> 16 & 0xff, clr >> 8 & 0xff, clr & 0xff, lyr);
}

void
tdk_setrgbclr(int r, int g, int b, int lyr)
{
	_tdk_writeansi("\033[%d8;2;%d;%d;%dm", lyr, r, g, b);
}

void
tdk_setwgt(int wgt)
{
	_tdk_writeansi(wgt == TDK_WGTDFT ? "\033[22m" : "\033[22;%dm", wgt);
}
