#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int main(void)
{
	struct termios attributes;
	signed char buffer[] = {-1, -1, -1, -1, -1};
	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag &= ~(ICANON | ECHO | ISIG);
	attributes.c_iflag &= ~IXON;
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	printf("Waiting for a key press to dump...");
	fflush(stdout);
	read(0, &buffer, sizeof(buffer));
	printf("\033[2K\033[1G");
	size_t lastOffset = sizeof(buffer) - 1;
	for (size_t offset = 0; offset < sizeof(buffer); ++offset) {
		if (buffer[offset] >= 0) {
			printf("%d", buffer[offset]);
		}
		if (offset < lastOffset) {
			putchar(' ');
		}
	}
	putchar('\n');
	attributes.c_lflag |= ICANON | ECHO | ISIG;
	attributes.c_iflag |= IXON;
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
	return 0;
}
