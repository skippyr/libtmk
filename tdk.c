#include "tdk.h"

#ifdef _WIN32
#define IN_PROC_ATTR (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT)
#else
#define IN_PROC_ATTR (ICANON | ECHO)
#endif

static int calc_dgts(int n);
static int set_attr(FILE *fd, int attr, int is_enb);
static void write_ansi(const char *msg);
static void write_ansif(int len, const char *fmt, ...);

static int calc_dgts(int n)
{
	int i = !n;
	for (; n; n /= 10)
		i++;
	return i;
}

static int set_attr(FILE *fd, int attr, int is_enb)
{
#ifdef _WIN32
	HANDLE h = GetStdHandle(fd == stdin ? STD_INPUT_HANDLE :
				fd == stdout ? STD_OUTPUT_HANDLE :
					       STD_ERROR_HANDLE);
	DWORD m;
	return !(GetConsoleMode(h, &m) &&
		 SetConsoleMode(h, is_enb ? m | attr : m & ~attr));
#else
	int n = fileno(fd);
	struct termios a;
	return !(!tcgetattr(n, &a) &&
		 (is_enb ? (a.c_lflag |= attr) : (a.c_lflag &= ~attr)) &&
		 !tcsetattr(n, TCSANOW, &a));
#endif
}

static void write_ansi(const char *msg)
{
#ifdef _WIN32
	(!set_attr(stdout, ENABLE_VIRTUAL_TERMINAL_PROCESSING, 1) ||
	 !set_attr(stderr, ENABLE_VIRTUAL_TERMINAL_PROCESSING, 1)) &&
#endif
	is_tty(stdout) && printf("%s", msg) ||
	is_tty(stderr) && fprintf(stderr, "%s", msg);
}

static void write_ansif(int len, const char *fmt, ...)
{
	va_list a;
	va_start(a, fmt);
	char *s = malloc(len);
	if (s) {
		vsnprintf(s, len, fmt, a);
		write_ansi(s);
		free(s);
	}
	va_end(a);
}

int get_cur_pos(int *col, int *ln)
{
	if (!is_tty(stdin) || !is_tty(stdout) && !is_tty(stderr))
		return 1;
	write_ansi("\33[6n");
	set_attr(stdin, IN_PROC_ATTR, 0);
	int c, l;
	while (scanf("\33[%d;%dR", &l, &c) != 2)
		getchar();
	if (col)
		*col = c - 1;
	if (ln)
		*ln = l - 1;
	set_attr(stdin, IN_PROC_ATTR, 1);
	return 0;
}

int get_size(int *col, int *ln)
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
	if (ioctl(1, TIOCGWINSZ, &s) && ioctl(2, TIOCGWINSZ, &s))
		return 1;
	if (col)
		*col = s.ws_col;
	if (ln)
		*ln = s.ws_row;
#endif
	return 0;
}

int is_tty(FILE *fd)
{
	return !!isatty(fileno(fd));
}

void beep(void)
{
	write_ansi("\7");
}

void clear(int cln)
{
	write_ansi(CLN_SCR == cln ? "\33[2J\33[1;1H" : "\33[2K\33[1G");
}

void set_256_clr(int lyr, int clr)
{
	write_ansif(12, CLR_DFT == clr ? "\33[%d9m" : "\33[%d8;5;%dm", lyr,
		    clr);
}

void set_alt_scr(int is_alt)
{
	write_ansi(is_alt ? "\33[?1049h\33[2J\33[1;1H" : "\33[?1049l");
}

void set_cur_pos(int col, int ln)
{
	write_ansif(calc_dgts(++col) + calc_dgts(++ln) + 5, "\33[%d;%dH", ln,
		    col);
}

void set_cur_sp(int sp)
{
	write_ansif(6, "\33[%d q", sp);
}

void set_cur_vis(int is_vis)
{
	write_ansi(is_vis ? "\33[?25h" : "\33[?25l");
}

void set_eff(int eff, int is_enb)
{
	for (int i = 3; i < 9; i++)
		if (1 << i & eff)
			write_ansif(6, "\33[%dm", is_enb ? i : i + 20);
}

void set_head(const char *head)
{
	write_ansif(strlen(head) + 6, "\33]0;%s\7", head);
}

void set_hex_clr(int lyr, int hex)
{
	set_rgb_clr(lyr, hex >> 16 & 0xff, hex >> 8 & 0xff, hex & 0xff);
}

void set_lum(int lum)
{
	write_ansif(8, LUM_DFT == lum ? "\33[%dm" : "\33[22;%dm", lum);
}

void set_rgb_clr(int lyr, int r, int g, int b)
{
	write_ansif(20, "\33[%d8;2;%d;%d;%dm", lyr, r, g, b);
}
