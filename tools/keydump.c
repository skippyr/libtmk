#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int
main(void)
{
	struct termios t;
	signed char buf[] = {-1, -1, -1, -1, -1};
	size_t i;
	tcgetattr(0, &t);
	t.c_lflag &= ~(ICANON | ECHO | ISIG);
	t.c_iflag &= ~IXON;
	tcsetattr(0, TCSANOW, &t);
	printf("Waiting for a key press to dump...");
	fflush(stdout);
	read(0, &buf, sizeof(buf));
	printf("\033[2K\033[1G");
	for (i = 0; i < sizeof(buf); ++i) {
		if (buf[i] >= 0)
			printf("%d", buf[i]);
		if (i < sizeof(buf) - 1)
			putchar(' ');
	}
	putchar('\n');
	t.c_lflag |= ICANON | ECHO | ISIG;
	t.c_iflag |= IXON;
	tcsetattr(0, TCSANOW, &t);
	return 0;
}
