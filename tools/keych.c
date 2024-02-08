#include <locale.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <wchar.h>

int main(void)
{
	int fl = fcntl(0, F_GETFL);
	int key;
	struct termios t;
	setlocale(LC_CTYPE, "C.UTF-8");
	tcgetattr(0, &t);
	t.c_lflag &= ~(ICANON | ECHO | ISIG);
	t.c_iflag &= ~IXON;
	tcsetattr(0, TCSANOW, &t);
	key = getwchar();
	printf("%d ", key);
	fcntl(0, F_SETFL, fl | O_NONBLOCK);
	while ((key = getwchar()) != WEOF)
		printf("%d ", key);
	printf("\n");
	t.c_lflag |= ICANON | ECHO | ISIG;
	t.c_iflag |= IXON;
	tcsetattr(0, TCSANOW, &t);
	fcntl(0, F_SETFL, fl);
	return (0);
}
