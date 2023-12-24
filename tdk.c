/* See LICENSE for copyright and license details. */
#include "tdk.h"

#define KEY(keyval, char0, char1, char2, char3)\
	if (trail[0] == char0 && trail[1] == char1 && trail[2] == char2 &&\
	    trail[3] == char3) {\
		key = keyval;\
		goto out;\
	}

static int ansi(char *msg);
static int wipein(void);
static int setraw(unsigned char isenb);
static int ansif(size_t len, char *fmt, ...);
static void setnblk(unsigned char isenb);

static int
ansi(char *msg)
{
	return !((isatty(1) && printf("%s", msg) >= 0 && !fflush(stdout)) ||
		 (isatty(2) && fprintf(stderr, "%s", msg) >= 0));
}

static int
wipein(void)
{
	if (fwide(stdin, 0) < 0 || !setlocale(LC_ALL, "C.UTF-8") || setraw(1))
		return 1;
	setnblk(1);
	while (getwchar() != WEOF);
	setraw(0);
	setnblk(0);
	return 0;
}

static int
setraw(unsigned char isenb)
{
	struct termios t;
	if (tcgetattr(0, &t))
		return 1;
	isenb ? (t.c_lflag &= ~(ICANON | ECHO)) : (t.c_lflag |= ICANON | ECHO);
	tcsetattr(0, TCSANOW, &t);
	return 0;
}

static int
ansif(size_t len, char *fmt, ...)
{
	char *msg;
	int ret;
	va_list args;
	if (!(msg = malloc(len)))
		return 1;
	va_start(args, fmt);
	vsnprintf(msg, len, fmt, args);
	ret = ansi(msg);
	va_end(args);
	free(msg);
	return ret;
}

static void
setnblk(unsigned char isenb)
{
	int fl = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, isenb ? fl | O_NONBLOCK : fl & ~O_NONBLOCK);
}

int
tdk_getcpos(unsigned short int *col, unsigned short int *row)
{
	unsigned short int tmpcol;
	unsigned short int tmprow;
	if (wipein() || setraw(1) || ansi("\33[6n"))
		return 1;
	wscanf(L"\33[%hu;%huR", &tmprow, &tmpcol);
	if (col)
		*col = --tmpcol;
	if (row)
		*row = --tmprow;
	setraw(0);
	return 0;
}

int
tdk_getkey(void)
{
	int key;
	int trail[4];
	unsigned char i;
	if ((!isatty(1) && !isatty(2)) || wipein())
		return WEOF;
	setraw(1);
	key = getwchar();
	setnblk(1);
	for (i = 0; i < 4; i++)
		trail[i] = getwchar();
	if (key == 27 && trail[0] != WEOF) {
		for (i = 65; i < 69; i++)
			KEY(i - 65 + tdk_KeyUpArr, 91, i, WEOF, WEOF);
		for (i = 80; i < 84; i++)
			KEY(i - 80 + tdk_KeyF1, 79, i, WEOF, WEOF);
		for (i = 65; i < 69; i++)
			KEY(i - 65 + tdk_KeyF1, 91, 91, i, WEOF);
		KEY(tdk_KeyF5, 91, 49, 53, 126);
		KEY(tdk_KeyF5, 91, 91, 69, WEOF);
		for (i = 55; i < 58; i++)
			KEY(i - 55 + tdk_KeyF6, 91, 49, i, 126);
		for (i = 48; i < 50; i++)
			KEY(i - 48 + tdk_KeyF9, 91, 50, i, 126);
		for (i = 51; i < 53; i++)
			KEY(i - 51 + tdk_KeyF11, 91, 50, i, 126);
		KEY(tdk_KeyIns, 91, 52, 104, WEOF);
		KEY(tdk_KeyIns, 91, 50, 126, WEOF);
		KEY(tdk_KeyDel, 91, 80, WEOF, WEOF);
		KEY(tdk_KeyDel, 91, 51, 126, WEOF);
		KEY(tdk_KeyEnd, 91, 52, 126, WEOF);
		KEY(tdk_KeyEnd, 91, 70, WEOF, WEOF);
		KEY(tdk_KeyHome, 91, 72, WEOF, WEOF);
		KEY(tdk_KeyHome, 91, 49, 126, WEOF);
		for (i = 53; i < 55; i++)
			KEY(i - 53 + tdk_KeyPgUp, 91, i, 126, WEOF);
		key = 0;
	}
out:
	wipein();
	return key < 0 || key == 9 || key == 10 || key > 26 ? key : 0;
}

int
tdk_getwdim(unsigned short int *col, unsigned short int *row)
{
	struct winsize w;
	if (ioctl(0, TIOCGWINSZ, &w) && ioctl(1, TIOCGWINSZ, &w) &&
	    ioctl(2, TIOCGWINSZ, &w))
		return 1;
	if (col)
		*col = w.ws_col;
	if (row)
		*row = w.ws_row;
	return 0;
}

int
tdk_setcpos(unsigned short int col, unsigned short int row)
{
	unsigned short int wcol;
	unsigned short int wrow;
	return tdk_getwdim(&wcol, &wrow) || ++col > wcol || ++row > wrow ||
	       ansif(13, "\33[%d;%dH", row, col);
}

void
tdk_beep(void)
{
	ansi("\7");
}

void
tdk_rwipe(void)
{
	ansi("\33[2K\33[1G");
}

void
tdk_setclr(int lyr, int clr)
{
	ansif(6, "\33[%d%dm", lyr, clr);
}

void
tdk_setcshp(int shp)
{
	ansif(6, "\33[%d q", shp);
}

void
tdk_setcvis(unsigned char isvis)
{
	ansi(isvis ? "\33[?25h" : "\33[?25l");
}

void
tdk_seteff(int eff, unsigned char isenb)
{
	unsigned char i;
	for (i = 3; i < 10; i++)
		if (1 << i & eff)
			ansif(6, "\33[%dm", isenb ? i : i + 20);
}

void
tdk_setlum(int lum)
{
	ansif(8, !lum ? "\33[22m" : "\33[22;%dm", lum);
}

void
tdk_setwalt(unsigned char isenb)
{
	ansi(isenb ? "\33[?1049h\33[2J\33[1;1H" : "\33[?1049l");
}
