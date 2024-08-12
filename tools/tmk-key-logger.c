#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <tmk.h>

#define PROGRAM_NAME "tmk-event-logger"
#define PROGRAM_VERSION "v1.0.1"
#define PARSE_OPTION(option_a, action_a)                                       \
  if (!strcmp(arguments[offset], "--" option_a)) {                             \
    action_a;                                                                  \
    return 0;                                                                  \
  }

static void writeHelpPage(void) {
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("❡ Usage");
  tmk_resetFontWeight();
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("    %s ", PROGRAM_NAME);
  tmk_resetFontWeight();
  tmk_resetFontColors();
  tmk_write("[");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("OPTION");
  tmk_resetFontEffects();
  tmk_writeLine("]...");
  tmk_writeLine("    Logs the bytes of terminal key events being listened.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("❡ Available Options");
  tmk_resetFontWeight();
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("    • ");
  tmk_resetFontColors();
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("--help: ");
  tmk_resetFontWeight();
  tmk_writeLine("writes these help instructions.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("❡ Homepage");
  tmk_resetFontWeight();
  tmk_write("    Its homepage is available on GitHub (");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("*");
  tmk_resetFontColors();
  tmk_writeLine("1).");
  tmk_writeLine("");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("    • *");
  tmk_resetFontColors();
  tmk_write("1: ");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_writeLine("https://github.com/skippyr/libtmk");
  tmk_resetFontEffects();
  tmk_resetFontColors();
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("❡ Help");
  tmk_resetFontWeight();
  tmk_writeLine("    If you need help related to this project, open a new "
                "issue in its issues");
  tmk_write("    page (");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("*");
  tmk_resetFontColors();
  tmk_write("1) or send me an e-mail (");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("*");
  tmk_resetFontColors();
  tmk_writeLine("2) describing what is going on.");
  tmk_writeLine("");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("    • *");
  tmk_resetFontColors();
  tmk_write("1: ");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_writeLine("https://github.com/skippyr/libtmk/issues");
  tmk_resetFontEffects();
  tmk_resetFontColors();
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("    • *");
  tmk_resetFontColors();
  tmk_write("2: ");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_writeLine("skippyr.developer@icloud.com");
  tmk_resetFontEffects();
  tmk_resetFontColors();
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("❡ Contributing");
  tmk_resetFontWeight();
  tmk_writeLine("    This project is open to review and possibly accept "
                "contributions, specially");
  tmk_writeLine("    fixes and suggestions. If you are interested, send your "
                "contribution to its");
  tmk_write("    pull requests page (");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("*");
  tmk_resetFontColors();
  tmk_write("1) or to my e-mail (");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("*");
  tmk_resetFontColors();
  tmk_writeLine("2).");
  tmk_writeLine("");
  tmk_writeLine("    By contributing to this project, you agree to license "
                "your work under the");
  tmk_writeLine("    same license that the project uses.");
  tmk_writeLine("");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("    • *");
  tmk_resetFontColors();
  tmk_write("1: ");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_writeLine("https://github.com/skippyr/libtmk/pulls");
  tmk_resetFontEffects();
  tmk_resetFontColors();
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("    • *");
  tmk_resetFontColors();
  tmk_write("2: ");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_writeLine("skippyr.developer@icloud.com");
  tmk_resetFontEffects();
  tmk_resetFontColors();
  tmk_writeLine("");
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("❡ License");
  tmk_resetFontWeight();
  tmk_writeLine("    This is free software licensed under the BSD-3-Clause "
                "License that comes");
  tmk_writeLine("    WITH NO WARRANTY. Refer to the LICENSE file that comes in "
                "its source code");
  tmk_writeLine("    for license and copyright details.");
}

static void writeVersionPage(void) {
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_write("%s ", PROGRAM_NAME);
  tmk_resetFontWeight();
  tmk_resetFontColors();
  tmk_setFontANSIColor(tmk_ANSIColor_DarkYellow, tmk_FontLayer_Foreground);
  tmk_write("%s ", PROGRAM_VERSION);
  tmk_resetFontColors();
  tmk_writeLine("compiled for %s %s.", tmk_OPERATING_SYSTEM,
                tmk_CPU_ARCHITECTURE);
  tmk_setFontANSIColor(tmk_ANSIColor_LightBlack, tmk_FontLayer_Foreground);
  tmk_write("Copyright (c) 2023, Sherman Rofeman <");
  tmk_setFontEffects(tmk_FontEffect_Underline);
  tmk_write("skippyr.developer@icloud.com");
  tmk_resetFontEffects();
  tmk_writeLine(">");
  tmk_resetFontColors();
  tmk_writeLine("");
  for (int color = 1; color < 16; ++color) {
    tmk_setFontANSIColor(color, tmk_FontLayer_Background);
    tmk_write("   ");
  }
  tmk_resetFontColors();
  tmk_writeLine("");
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
  tmk_write("For more info, use it with the ");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_write("--help");
  tmk_resetFontColors();
  tmk_writeLine(" option.");
  tmk_writeLine("");
  tmk_writeLine("Press the \x1b[33mEscape\x1b[39m key to exit.");
  tmk_writeLine("");
  int index = 0;
  while (true) {
    tmk_write("[Event #");
    tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
    tmk_write("%d", index++);
    tmk_resetFontColors();
    tmk_write("] ");
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
