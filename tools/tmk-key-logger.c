#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <tmk.h>

#define PROGRAM_NAME "tmk-event-logger"
#define PARSE_OPTION(option_a, action_a)                                       \
  if (!strcmp(arguments[offset], "--" option_a)) {                             \
    action_a;                                                                  \
  }

static void writeHelp(void) {
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("Usage:");
  tmk_resetFontWeight();
  tmk_write(" %s [", PROGRAM_NAME);
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("OPTION");
  tmk_resetFontEffects();
  tmk_writeLine("]...");
  tmk_writeLine("Logs the bytes related to terminal key events.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE OPTIONS\n");
  tmk_resetFontWeight();
  tmk_write("    ");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkGreen, tmk_FontLayer_Foreground);
  tmk_write("--help");
  tmk_resetFontColors();
  tmk_writeLine("  shows these help instructions.");
  tmk_writeLine("");
  tmk_writeLine("This software is bundled with libtmk.");
  tmk_write("For more info, visit: <");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("https://github.com/skippyr/libtmk");
  tmk_resetFontEffects();
  tmk_writeLine(">.");
}

int main(int totalArguments, const char **arguments) {
  for (int offset = 0; offset < totalArguments; ++offset) {
    if (!strcmp(arguments[offset], "--help")) {
      writeHelp();
      return 0;
    }
  }
  struct termios attributes;
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  tcgetattr(STDIN_FILENO, &attributes);
  attributes.c_lflag &= ~(ICANON | ECHO | ISIG);
  attributes.c_iflag &= ~IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine(PROGRAM_NAME);
  tmk_writeLine("");
  tmk_resetFontWeight();
  tmk_writeLine("Waiting for terminal key events to start logging.");
  tmk_write("For more info, use it with the ");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkGreen, tmk_FontLayer_Foreground);
  tmk_write("--help");
  tmk_resetFontColors();
  tmk_writeLine(" option.");
  tmk_writeLine("");
  tmk_writeLine("Press the \x1b[33mEscape\x1b[39m key to exit.");
  tmk_writeLine("");
  while (true) {
    int key = getchar();
    tmk_write("%d", key);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    if (key == 27) {
      if ((key = getchar()) == EOF) {
        tmk_writeLine("");
        break;
      }
      tmk_write(" %d", key);
    }
    while ((key = getchar()) != EOF) {
      tmk_write(" %d", key);
    }
    tmk_writeLine("");
    fcntl(STDIN_FILENO, F_SETFL, flags);
  };
  attributes.c_lflag |= ICANON | ECHO | ISIG;
  attributes.c_iflag |= IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  fcntl(STDIN_FILENO, F_SETFL, flags);
  tmk_writeLine("");
  tmk_writeLine("Exited.");
  return 0;
}
