#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main(void)
{
    struct termios attributes;
    tcgetattr(STDIN_FILENO, &attributes);
    attributes.c_lflag &= ~(ICANON | ECHO | ISIG);
    attributes.c_iflag &= ~IXON;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    printf("Waiting for a key press to dump...");
    fflush(stdout);
    signed char buffer[] = {-1, -1, -1, -1, -1};
    read(STDIN_FILENO, &buffer, sizeof(buffer));
    printf("\033[2K\033[1G");
    for (size_t index = 0; index < sizeof(buffer); ++index)
    {
        if (buffer[index] >= 0)
        {
            printf("%d", buffer[index]);
        }
        if (index < sizeof(buffer) - 1)
        {
            putchar(' ');
        }
    }
    putchar('\n');
    attributes.c_lflag |= ICANON | ECHO | ISIG;
    attributes.c_iflag |= IXON;
    tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
    return 0;
}
