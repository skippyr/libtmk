#include <fcntl.h>
#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>
#include "tdk.h"

#define _TDK_HASLOCALECACHE (_tdk_cache_g & 1 << 6)
#define _TDK_HASTTYCACHE (_tdk_cache_g & 1 << 7)
#define _TDK_ISTTY(fd_m) (_tdk_cache_g & 1 << fd_m)
#define _TDK_PARSECH(cond_m, key_m) \
	if (cond_m) { \
		*key = key_m; \
		goto end; \
	}

static int _tdk_ansif(char *fmt, ...);
static void _tdk_cachelocale(void);
static void _tdk_cachetty(void);

static char _tdk_cache_g = 0;

static int _tdk_ansif(char *fmt, ...)
{
	va_list args;
	_tdk_cachetty();
	if (!_TDK_ISTTY(1) && !_TDK_ISTTY(2))
		return (-1);
	va_start(args, fmt);
	vfprintf(_TDK_ISTTY(1) ? stdout : stderr, fmt, args);
	va_end(args);
	return (0);
}

static void _tdk_cachelocale(void)
{
	if (!_TDK_HASLOCALECACHE) {
		setlocale(LC_CTYPE, "C.UTF-8");
		_tdk_cache_g |= 1 << 6;
	}
}

static void _tdk_cachetty(void)
{
	if (!_TDK_HASTTYCACHE)
		_tdk_cache_g |= isatty(0) | isatty(1) << 1 | isatty(2) << 2 | 1 << 7;
}

void tdk_beep(void)
{
	_tdk_ansif("\007");
}

void tdk_clearin(void)
{
	int fl = fcntl(0, F_GETFL);
	struct termios t;
	tcgetattr(0, &t);
	t.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &t);
	fcntl(0, F_SETFL, fl | O_NONBLOCK);
	while (getwchar() != WEOF);
	t.c_lflag |= ICANON | ECHO;
	tcsetattr(0, TCSANOW, &t);
	fcntl(0, F_SETFL, fl);
}

void tdk_clearln(void)
{
	_tdk_ansif("\033[2K\033[1G");
}

int tdk_getcpos(unsigned short int *col, unsigned short int *row)
{
	unsigned short int tmpcol;
	unsigned short int tmprow;
	struct termios t;
	if (_tdk_ansif("\033[6n") || tcgetattr(0, &t))
		return (-1);
	t.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &t);
	wscanf(L"\033[%hu;%huR", &tmprow, &tmpcol);
	if (col)
		*col = --tmpcol;
	if (row)
		*row = --tmprow;
	t.c_lflag |= ICANON | ECHO;
	tcsetattr(0, TCSANOW, &t);
	return (0);
}

int tdk_getwdim(unsigned short int *col, unsigned short int *row)
{
	struct winsize w;
	if (ioctl(1, TIOCGWINSZ, &w) && ioctl(0, TIOCGWINSZ, &w) &&
		ioctl(2, TIOCGWINSZ, &w))
		return (-1);
	if (col)
		*col = w.ws_col;
	if (row)
		*row = w.ws_row;
	return (0);
}

int tdk_readkey(int *key, char *mod)
{
	int ch[8];
	int fl;
	int i;
	char tmpmod = 0;
	struct termios t;
	_tdk_cachetty();
	if (!_TDK_ISTTY(0) || (!_TDK_ISTTY(1) && !_TDK_ISTTY(2)))
		return (-1);
	_tdk_cachelocale();
	tcgetattr(0, &t);
	t.c_lflag &= ~(ICANON | ECHO | ISIG);
	t.c_iflag &= ~IXON;
	tcsetattr(0, TCSANOW, &t);
	*ch = getwchar();
	fl = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, fl | O_NONBLOCK);
	if (*ch == 27 && ((ch[1] = getwchar()) == 79 || ch[1] == 91)) {
		for (i = 2; i < 8; i++)
			ch[i] = getwchar();
		while (getwchar() != WEOF);
		_TDK_PARSECH(ch[2] >= 65 && ch[2] <= 68, ch[2] - 65 + TDK_KEYUPARR);
		_TDK_PARSECH(ch[3] == 126, ch[2] - 49 + TDK_KEYHOME);
		_TDK_PARSECH(ch[3] == 104 || (ch[1] == 91 && ch[2] == 80),
					 !(ch[3] == 104) + TDK_KEYINS);
		_TDK_PARSECH(ch[2] == 70 || ch[2] == 72, ch[2] == 72 ? TDK_KEYHOME :
					 TDK_KEYEND);
		_TDK_PARSECH(ch[2] >= 80 && ch[2] <= 83, ch[2] - 80 + TDK_KEYF1);
		_TDK_PARSECH(ch[3] >= 65 && ch[3] <= 69, ch[3] - 65 + TDK_KEYF1);
		_TDK_PARSECH(ch[4] == 126, ch[3] == 53 ? TDK_KEYF5 : ch[3] >= 55 &&
					 ch[3] <= 57 ? ch[3] - 55 + TDK_KEYF6 : ch[3] == 48 ||
					 ch[3] == 49 ? ch[3] - 48 + TDK_KEYF9 : ch[3] - 51 +
					 TDK_KEYF11);
		_TDK_PARSECH(ch[7] == 117, (ch[6] == 50) + TDK_KEYPRTSC);
		*key = 0;
		goto end;
	}
	if ((*key = (tmpmod = *ch == 27 && ch[1] >= 0) ? ch[1] : *ch) >= 0 &&
		*key <= 26 && *key != 9 && *key != 10) {
		*key = !*key ? TDK_KEYSPACE : *key + 96;
		tmpmod |= TDK_MODCTRL;
	}
 end:
	if (mod)
		*mod = tmpmod;
	t.c_lflag |= ICANON | ECHO | ISIG;
	t.c_iflag |= IXON;
	tcsetattr(0, TCSANOW, &t);
	fcntl(0, F_SETFL, fl);
	return (0);
}

void tdk_set256clr(int clr, int lyr)
{
	_tdk_ansif(clr < 0 ? "\033[%d9m" : "\033[%d8;5;%dm", lyr, clr);
}

void tdk_setcpos(unsigned short int col, unsigned short int row)
{
	_tdk_ansif("\033[%d;%dH", ++row, ++col);
}

void tdk_setcshp(int shp)
{
	_tdk_ansif("\033[%d q", shp);
}

void tdk_setcvis(int isenb)
{
	_tdk_ansif("\033[?25%c", isenb ? 'h' : 'l');
}

void tdk_seteff(int eff, int isenb)
{
	int i;
	for (i = 3; i < 10; i++)
		if (eff & 1 << i)
			_tdk_ansif("\033[%dm", isenb ? i : i + 20);
}

void tdk_sethexclr(int hex, int lyr)
{
	tdk_setrgbclr(hex >> 16 & 0xff, hex >> 8 & 0xff, hex & 0xff, lyr);
}

void tdk_setlum(int lum)
{
	_tdk_ansif(!lum ? "\033[22m" : "\033[22;%dm", lum);
}

void tdk_setrgbclr(int r, int g, int b, int lyr)
{
	_tdk_ansif("\033[%d8;2;%d;%d;%dm", lyr, r, g, b);
}

void tdk_setwalt(int isenb)
{
	_tdk_ansif(isenb ? "\033[?1049h\033[2J\033[1;1H" : "\033[?1049l");
}
