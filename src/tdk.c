#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>

#include "tdk.h"

#define _tdk_IS_TTY(stream_a) (_tdk_cache_g & 1 << stream_a)
#define _tdk_PARSE_KEY(condition_a, key_a) \
	if (condition_a) { \
		event->key = key_a; \
		break; \
	}

static void _tdk_cacheTTY(void);
static int _tdk_writeANSI(const char *format, ...);

static char _tdk_cache_g = 0;

static void _tdk_cacheTTY(void)
{
	if (!(_tdk_cache_g & 1 << 7)) {
		_tdk_cache_g |= isatty(STDIN_FILENO) | isatty(STDOUT_FILENO) << 1 |
						isatty(STDERR_FILENO) << 2 | 1 << 7;
	}
}

static int _tdk_writeANSI(const char *format, ...)
{
	_tdk_cacheTTY();
	if (!_tdk_IS_TTY(STDOUT_FILENO) && !_tdk_IS_TTY(STDERR_FILENO)) {
		return -1;
	}
	va_list arguments;
	va_start(arguments, format);
	vfprintf(_tdk_IS_TTY(STDOUT_FILENO) ? stdout : stderr, format, arguments);
	va_end(arguments);
	return 0;
}

void tdk_clearCursorLine(void)
{
	_tdk_writeANSI("\033[2K\033[1G");
}

void tdk_clearInputBuffer(void)
{
	struct termios attributes;
	if (tcgetattr(STDIN_FILENO, &attributes)) {
		return;
	}
	int flags = fcntl(STDIN_FILENO, F_GETFL);
	attributes.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	while (getchar() != EOF) {
	}
	attributes.c_lflag |= ICANON | ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	fcntl(STDIN_FILENO, F_SETFL, flags);
}

int tdk_getCursorCoordinate(struct tdk_Coordinate *coordinate)
{
	tdk_clearInputBuffer();
	struct termios attributes;
	if (_tdk_writeANSI("\033[6n") || tcgetattr(STDIN_FILENO, &attributes)) {
		return -1;
	}
	attributes.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	int totalMatches = scanf("\033[%hu;%huR", &coordinate->row, &coordinate->column);
	--coordinate->row;
	--coordinate->column;
	attributes.c_lflag |= ICANON | ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	return -(totalMatches != 2);
}

int tdk_getScreenDimensions(struct tdk_Dimensions *dimensions)
{
	struct winsize size;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) &&
		ioctl(STDIN_FILENO, TIOCGWINSZ, &size) &&
		ioctl(STDERR_FILENO, TIOCGWINSZ, &size)) {
		return -1;
	}
	dimensions->totalColumns = size.ws_col;
	dimensions->totalRows = size.ws_row;
	return 0;
}

int tdk_readKeyEvent(struct tdk_KeyEvent *event)
{
	_tdk_cacheTTY();
	if (!_tdk_IS_TTY(STDIN_FILENO) || fwide(stdin, 0) > 0 ||
		(!_tdk_IS_TTY(STDOUT_FILENO) && !_tdk_IS_TTY(STDERR_FILENO))) {
		return -1;
	}
	struct termios attributes;
	tcgetattr(STDIN_FILENO, &attributes);
	int flags = fcntl(STDIN_FILENO, F_GETFL);
	attributes.c_lflag &= ~(ICANON | ECHO | ISIG);
	attributes.c_iflag &= ~IXON;
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	char buffer[5];
	while (1) {
		memset(buffer, 0, sizeof(buffer));
		buffer[0] = getchar();
		fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
		if (buffer[0] == 27 && ((buffer[1] = getchar()) == 79 || buffer[1] == 91)) {
			for (int offset = 2; offset < 5; ++offset) {
				buffer[offset] = getchar();
			}
			while (getchar() != EOF) {
			}
			_tdk_PARSE_KEY(buffer[2] >= 65 && buffer[2] <= 68,
						   buffer[2] - 65 + tdk_Key_UpArrow);
			_tdk_PARSE_KEY(buffer[3] == 126, buffer[2] - 49 + tdk_Key_Home);
			_tdk_PARSE_KEY(buffer[3] == 104 || (buffer[1] == 91 && buffer[2] == 80),
						   !(buffer[3] == 104) + tdk_Key_Home);
			_tdk_PARSE_KEY(buffer[2] == 70 || buffer[2] == 72,
						   buffer[2] == 72 ? tdk_Key_Home : tdk_Key_End);
			_tdk_PARSE_KEY(buffer[2] >= 80 && buffer[2] <= 83,
						   buffer[2] - 80 + tdk_Key_F1);
			_tdk_PARSE_KEY(buffer[3] >= 65 && buffer[3] <= 69,
						   buffer[3] - 65 + tdk_Key_F1);
			_tdk_PARSE_KEY(buffer[4] == 126, buffer[3] == 53 ? tdk_Key_F5
											 : buffer[3] >= 55 && buffer[3] <= 57
												 ? buffer[3] - 55 + tdk_Key_F6
											 : buffer[3] == 48 || buffer[3] == 49
												 ? buffer[3] - 48 + tdk_Key_F9
												 : buffer[3] - 51 + tdk_Key_F11);
			fcntl(STDIN_FILENO, F_SETFL, flags);
			continue;
		}
		if (buffer[0] & 1 << 7) {
			for (int offset = 1;
				 offset < 1 + !!(buffer[0] & 1 << 6) + !!(buffer[0] & 1 << 5) +
							  !!(buffer[0] & 1 << 4);
				 ++offset) {
				buffer[offset] = getchar();
			}
			event->key = *(int *)buffer;
		} else if ((event->key = (event->modifiers = buffer[0] == 27 && buffer[1] != EOF)
									 ? buffer[1]
									 : buffer[0]) >= 0 &&
				   event->key <= 26 && event->key != tdk_Key_Tab &&
				   event->key != tdk_Key_Enter) {
			event->key = !event->key ? tdk_Key_Space : event->key + 96;
			event->modifiers |= tdk_Modifier_Ctrl;
		}
		break;
	}
	attributes.c_lflag |= ICANON | ECHO | ISIG;
	attributes.c_iflag |= IXON;
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	fcntl(STDIN_FILENO, F_SETFL, flags);
	return 0;
}

void tdk_ringBell(void)
{
	_tdk_writeANSI("\7");
}

void tdk_set256Color(int color, int layer)
{
	_tdk_writeANSI(color == tdk_Color_Default ? "\033[%d9m" : "\033[%d8;5;%dm", layer,
				   color);
}

void tdk_setAlternateScreen(int isToEnable)
{
	_tdk_writeANSI(isToEnable ? "\033[?1049h\033[2J\033[1;1H" : "\033[?1049l");
}

void tdk_setCursorCoordinate(struct tdk_Coordinate coordinate)
{
	_tdk_writeANSI("\033[%hu;%huH", coordinate.row + 1, coordinate.column + 1);
}

void tdk_setCursorShape(int shape)
{
	_tdk_writeANSI("\033[%d q", shape);
}

void tdk_setCursorVisibility(int isToShow)
{
	_tdk_writeANSI("\033[?25%c", isToShow ? 'h' : 'l');
}

void tdk_setEffect(int effect, int isToEnable)
{
	for (int ansi = 0; ansi < 9; ++ansi) {
		if (effect & 1 << ansi) {
			_tdk_writeANSI("\033[%dm", isToEnable ? ansi : ansi + 20);
		}
	}
}

void tdk_setHexColor(int color, int layer)
{
	tdk_setRGBColor((struct tdk_RGB){color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff},
					layer);
}

void tdk_setRGBColor(struct tdk_RGB color, int layer)
{
	_tdk_writeANSI("\033[%d8;2;%d;%d;%dm", layer, color.red, color.green, color.blue);
}

void tdk_setWeight(int weight)
{
	_tdk_writeANSI(weight == tdk_Weight_Default ? "\033[22m" : "\033[22;%dm", weight);
}
