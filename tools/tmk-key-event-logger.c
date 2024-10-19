#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <tmk.h>
#include <unistd.h>

#define SOFTWARE_NAME "tmk-key-event-logger"
#define SOFTWARE_VERSION "1.0.2"
#define SOFTWARE_AUTHOR_NAME "Sherman Rofeman"
#define SOFTWARE_AUTHOR_EMAIL "skippyr.developer@icloud.com"
#define SOFTWARE_REPOSITORY_URL "https://github.com/skippyr/libtmk"
#define SOFTWARE_LICENSE "BSD-3-Clause License"
#define SOFTWARE_CREATION_YEAR 2023
#define PARSE_OPTION(option_a, action_a)                                       \
  if (!strcmp(arguments[offset], "--" option_a)) {                             \
    action_a;                                                                  \
    return 0;                                                                  \
  }

static void writeHelpPage(void) {
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("Usage:");
  tmk_resetFontWeight();
  tmk_write(" %s [", SOFTWARE_NAME);
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("OPTION");
  tmk_resetFontEffects();
  tmk_writeLine("]...");
  tmk_writeLine("Logs the bytes of terminal key events being listened.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("AVAILABLE OPTIONS");
  tmk_resetFontWeight();
  tmk_writeLine("    --help        Shows the software help instructions.");
  tmk_writeLine("    --version     Shows the software version.");
}

static void writeVersionPage(void) {
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write(SOFTWARE_NAME);
  tmk_resetFontWeight();
  tmk_writeLine(" %s (running on %s %s)", SOFTWARE_VERSION,
                tmk_OPERATING_SYSTEM, tmk_CPU_ARCHITECTURE);
  tmk_write("%s. Copyright © %d %s <", SOFTWARE_LICENSE, SOFTWARE_CREATION_YEAR,
            SOFTWARE_AUTHOR_NAME);
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write(SOFTWARE_AUTHOR_EMAIL);
  tmk_resetFontEffects();
  tmk_writeLine(">.");
  tmk_writeLine("");
  tmk_write("Software repository available at <");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write(SOFTWARE_REPOSITORY_URL);
  tmk_resetFontEffects();
  tmk_writeLine(">.");
}

int main(int totalArguments, const char **arguments) {
  for (int offset = 0; offset < totalArguments; ++offset) {
    PARSE_OPTION("help", writeHelpPage());
    PARSE_OPTION("version", writeVersionPage());
  }
  struct termios attributes;
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  tcgetattr(STDIN_FILENO, &attributes);
  attributes.c_lflag &= ~(ICANON | ECHO | ISIG);
  attributes.c_iflag &= ~IXON;
  tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
  tmk_writeLine("Waiting for terminal key events to start logging.");
  tmk_writeLine("For more info, use it with the --help option.");
  tmk_writeLine("");
  tmk_writeLine("Press the [Escape] key to exit.");
  tmk_writeLine("");
  int index = 0;
  while (1) {
    tmk_write("[Event %d] ", index++);
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
  return 0;
}
