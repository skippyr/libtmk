#ifdef _WIN32
#error This software can only be compiled on macOS and Linux.
#endif
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <tmk.h>
#include <unistd.h>

#define NAME "tmk-evtlog"
#define VERSION "1.0.0"
#define REPO "https://github.com/skippyr/libtmk"
#define UEOF 255

static void writehelp(void);
static void writeversion(void);
static void setraw(int israw);
static void setblk(int isblk);
static int parseargs(int argc, const char **argv);
static void logevts(void);

static void
writehelp(void)
{
  tmk_write("Usage: %s [OPTIONS]...\n", NAME);
  tmk_write("Logs the bytes of key events on macOS and Linux.\n\n");
  tmk_write("AVAILABLE OPTIONS\n");
  tmk_write("  --help     shows the software help instructions.\n");
  tmk_write("  --version  shows the software version.\n");
}

static void
writeversion(void)
{
  tmk_write("%s %s\n", NAME, VERSION);
  tmk_write("%s\n", REPO);
}

static void
setraw(int israw)
{
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag = israw ? t.c_lflag & ~(ICANON | ECHO | ISIG)
                    : t.c_lflag | ICANON | ECHO | ISIG;
  t.c_iflag = israw ? t.c_iflag & ~IXON : t.c_iflag | IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

static void
setblk(int isblk)
{
  int f = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL, isblk ? f & ~O_NONBLOCK : f | O_NONBLOCK);
}

static int
parseargs(int argc, const char **argv)
{
  for (int i = 1; i < argc; ++i) {
    if (!strcmp(argv[i], "--help")) {
      writehelp();
      return -1;
    } else if (!strcmp(argv[i], "--version")) {
      writeversion();
      return -1;
    }
  }
  return 0;
}

static void
logevts(void)
{
  tmk_write("Waiting for key events to log\n");
  tmk_write("Press the [Escape] key to exit.\n\n");
  setraw(1);
  size_t i = 0;
  while (1) {
    unsigned char byte = getchar();
    tmk_write("%zu: %u", i++, byte);
    setblk(0);
    if (byte == 27) {
      if ((byte = getchar()) == UEOF) {
        tmk_write("\n");
        setblk(1);
        break;
      }
      tmk_write(" %u", byte);
    }
    while ((byte = getchar()) != UEOF) {
      tmk_write(" %u", byte);
    }
    tmk_write("\n");
    setblk(1);
  }
  setraw(0);
  tmk_write("\nExited.\n");
}

int
main(int argc, const char **argv)
{
  if (parseargs(argc, argv)) {
    return 0;
  }
  logevts();
  return 0;
}
