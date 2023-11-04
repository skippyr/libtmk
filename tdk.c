#include "tdk.h"

#ifdef _WIN32
#define IN_PROC_ATTR (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT)
#else
#define IN_PROC_ATTR (ICANON | ECHO)
#endif

static int countdgts(int n);
static int setflags(FILE *fd, int flags, int isenb);
static void writefansi(unsigned long len, char *fmt, ...);
static void writeansi(char *msg);

static int
countdgts(int n)
{
	int i = !n;
	for (; n; n /= 10)
		i++;
	return i;
}

static int
setflags(FILE *fd, int flags, int isenb)
{
#ifdef _WIN32
	HANDLE h = GetStdHandle(fd == stdin ? STD_INPUT_HANDLE :
				fd == stdout ? STD_OUTPUT_HANDLE :
					       STD_ERROR_HANDLE);
	DWORD m;
	return !(GetConsoleMode(h, &m) &&
	         SetConsoleMode(h, isenb ? m | flags : m & ~flags));
#else
	int n = fileno(fd);
	struct termios a;
	return !(!tcgetattr(n, &a) &&
		 (isenb ? (a.c_lflag |= flags) : (a.c_lflag &= ~flags)) &&
		 !tcsetattr(n, TCSANOW, &a));
#endif
}

static void
writefansi(unsigned long len, char *fmt, ...)
{
	va_list a;
	va_start(a, fmt);
	char *s = malloc(len);
	if (s) {
		vsnprintf(s, len, fmt, a);
		writeansi(s);
		free(s);
	}
	va_end(a);
}

static void
writeansi(char *msg)
{
#ifdef _WIN32
	(!setflags(stdout, ENABLE_VIRTUAL_TERMINAL_PROCESSING, 1) ||
	 !setflags(stderr, ENABLE_VIRTUAL_TERMINAL_PROCESSING, 1)) &&
#endif
	istty(stdout) && printf("%s", msg) && !fflush(stdout) ||
	istty(stderr) && fprintf(stderr, "%s", msg);
}

int
istty(FILE *fd)
{
	return !!isatty(fileno(fd));
}

int
getcurpos(int *col, int *ln)
{
	if (!istty(stdin) || !istty(stdout) && !istty(stderr))
		return 1;
	writeansi("\33[6n");
	setflags(stdin, IN_PROC_ATTR, 0);
	int c, l;
	while (scanf("\33[%d;%dR", &l, &c) != 2)
		getchar();
	setflags(stdin, IN_PROC_ATTR, 1);
	if (col)
		*col = c - 1;
	if (ln)
		*ln = l - 1;
	return 0;
}

int
getsize(int *col, int *ln)
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO i;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &i) &&
	    !GetConsoleScreenBufferInfo(GetStdHandle(STD_ERROR_HANDLE), &i))
		return 1;
	if (col)
		*col = i.srWindow.Right - i.srWindow.Left + 1;
	if (ln)
		*ln = i.srWindow.Bottom - i.srWindow.Top + 1;
#else
	struct winsize s;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &s) &&
	    ioctl(STDERR_FILENO, TIOCGWINSZ, &s))
		return 1;
	if (col)
		*col = s.ws_col;
	if (ln)
		*ln = s.ws_row;
#endif
	return 0;
}

void
beep(void)
{
	writeansi("\7");
}

void
clear(int cln)
{
	writeansi(CLN_SCR == cln ? "\33[2J\33[1;1H" : "\33[2K\33[1G");
}

void
movecur(int drt, int stp)
{
	writefansi(countdgts(stp) + 4, "\33[%d%c", stp, drt);
}

void
set256clr(int lyr, int clr)
{
	writefansi(12, CLR_DFT == clr ? "\33[%d9m" : "\33[%d8;5;%dm", lyr, clr);
}

void
setaltscr(int isenb)
{
	writeansi(isenb ? "\33[?1049h\33[2J\33[1;1H" : "\33[?1049l");
}

void
setcurpos(int col, int ln)
{
	writefansi(countdgts(++col) + countdgts(++ln) + 5, "\33[%d;%dH", ln,
	           col);
}

void
setcursp(int sp)
{
	writefansi(6, "\33[%d q", sp);
}

void
setcurvis(int isvis)
{
	writeansi(isvis ? "\33[?25h" : "\33[?25l");
}

void
seteff(int eff, int isenb)
{
	for (int i = 3; i < 9; i++)
		if (1 << i & eff)
			writefansi(6, "\33[%dm", isenb ? i : i + 20);
}

void
sethexclr(int lyr, int hex)
{
	setrgbclr(lyr, hex >> 16 & 0xff, hex >> 8 & 0xff, hex & 0xff);
}

void
setlum(int lum)
{
	writefansi(8, LUM_DFT == lum ? "\33[%dm" : "\33[22;%dm", lum);
}

void
setname(char *name)
{
	writefansi(strlen(name) + 6, "\33]0;%s\7", name);
}

void
setrgbclr(int lyr, int r, int g, int b)
{
	writefansi(20, "\33[%d8;2;%d;%d;%dm", lyr, r, g, b);
}
