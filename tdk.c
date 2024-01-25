#include "tdk.h"

#define PARSETRAIL(key, c0, c1, c2, c3)\
	if (trail[0] == c0 && trail[1] == c1 && trail[2] == c2 &&\
	    trail[3] == c3) {\
		head = key;\
		goto out;\
	}

static int ansi(char *msg);
static int ansif(int len, char *fmt, ...);
static int clearin(void);
static int setraw(int isenb);
static void setnblk(int isenb);

static int
ansi(char *msg)
{
	return !((isatty(1) && printf("%s", msg) >= 0 && !fflush(stdout)) ||
		 (isatty(2) && fprintf(stderr, "%s", msg) >= 0));
}

static int
ansif(int len, char *fmt, ...)
{
	char *msg;
	int res;
	va_list args;
	if (!(msg = malloc(len)))
		return 1;
	va_start(args, fmt);
	vsnprintf(msg, len, fmt, args);
	res = ansi(msg);
	va_end(args);
	free(msg);
	return res;
}

static int
clearin(void)
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
setraw(int isenb)
{
	struct termios t;
	if (tcgetattr(0, &t))
		return 1;
	isenb ? (t.c_lflag &= ~(ICANON | ECHO)) : (t.c_lflag |= ICANON | ECHO);
	tcsetattr(0, TCSANOW, &t);
	return 0;
}

static void
setnblk(int isenb)
{
	int fl = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, isenb ? fl | O_NONBLOCK : fl & ~O_NONBLOCK);
}

int
tdk_getcpos(int *col, int *ln)
{
	int tmpcol;
	int tmpln;
	if (clearin() || setraw(1) || ansi("\33[6n"))
		return 1;
	wscanf(L"\33[%d;%dR", &tmpln, &tmpcol);
	if (col)
		*col = --tmpcol;
	if (ln)
		*ln = --tmpln;
	setraw(0);
	return 0;
}

int
tdk_getkey(void)
{
	int head;
	int i;
	int trail[4];
	if ((!isatty(1) && !isatty(2)) || clearin())
		return WEOF;
	setraw(1);
	head = getwchar();
	setnblk(1);
	for (i = 0; i < 4; i++)
		trail[i] = getwchar();
	if (head == 27 && trail[0] != WEOF) {
		for (i = 65; i < 69; i++)
			PARSETRAIL(i - 65 + tdk_KeyUpArr, 91, i, WEOF, WEOF);
		for (i = 80; i < 84; i++)
			PARSETRAIL(i - 80 + tdk_KeyF1, 79, i, WEOF, WEOF);
		for (i = 65; i < 69; i++)
			PARSETRAIL(i - 65 + tdk_KeyF1, 91, 91, i, WEOF);
		PARSETRAIL(tdk_KeyF5, 91, 49, 53, 126);
		PARSETRAIL(tdk_KeyF5, 91, 91, 69, WEOF);
		for (i = 55; i < 58; i++)
			PARSETRAIL(i - 55 + tdk_KeyF6, 91, 49, i, 126);
		for (i = 48; i < 50; i++)
			PARSETRAIL(i - 48 + tdk_KeyF9, 91, 50, i, 126);
		for (i = 51; i < 53; i++)
			PARSETRAIL(i - 51 + tdk_KeyF11, 91, 50, i, 126);
		PARSETRAIL(tdk_KeyIns, 91, 52, 104, WEOF);
		PARSETRAIL(tdk_KeyIns, 91, 50, 126, WEOF);
		PARSETRAIL(tdk_KeyDel, 91, 80, WEOF, WEOF);
		PARSETRAIL(tdk_KeyDel, 91, 51, 126, WEOF);
		PARSETRAIL(tdk_KeyEnd, 91, 52, 126, WEOF);
		PARSETRAIL(tdk_KeyEnd, 91, 70, WEOF, WEOF);
		PARSETRAIL(tdk_KeyHome, 91, 72, WEOF, WEOF);
		PARSETRAIL(tdk_KeyHome, 91, 49, 126, WEOF);
		for (i = 53; i < 55; i++)
			PARSETRAIL(i - 53 + tdk_KeyPgUp, 91, i, 126, WEOF);
		head = 0;
	}
out:
	clearin();
	return head < 0 || head == 9 || head == 10 || head > 26 ? head : 0;
}

int
tdk_getwdim(int *col, int *ln)
{
	struct winsize w;
	if (ioctl(0, TIOCGWINSZ, &w) && ioctl(1, TIOCGWINSZ, &w) &&
	    ioctl(2, TIOCGWINSZ, &w))
		return 1;
	if (col)
		*col = w.ws_col;
	if (ln)
		*ln = w.ws_row;
	return 0;
}

int
tdk_setcpos(int col, int ln)
{
	int wcol;
	int wln;
	return (tdk_getwdim(&wcol, &wln) || ++col < 1 || col > wcol || ++ln < 1 ||
		ln > wln || ansif(13, "\33[%d;%dH", ln, col));
}

void
tdk_beep(void)
{
	ansi("\7");
}

void
tdk_clearln(void)
{
	ansi("\33[2K\33[1G");
}

void
tdk_setclr(int clr, int lyr)
{
	ansif(6, "\33[%d%dm", lyr, clr);
}

void
tdk_setcshp(int shp)
{
	ansif(6, "\33[%d q", shp);
}

void
tdk_setcvis(int isvis)
{
	ansi(isvis ? "\33[?25h" : "\33[?25l");
}

void
tdk_seteff(int eff, int isenb)
{
	int i;
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
tdk_setwalt(int isenb)
{
	ansi(isenb ? "\33[?1049h\33[2J\33[1;1H" : "\33[?1049l");
}
