#include <tmk.h>
#include <string.h>

#define SOFTWARE_NAME "tmk-event-logger"
#define SOFTWARE_VERSION "v1.0.0"
#define SOFTWARE_AUTHOR_NAME "Sherman Rofeman"
#define SOFTWARE_AUTHOR_EMAIL "skippyr.developer@icloud.com"
#define SOFTWARE_LICENSE "BSD-3-Clause License"
#define SOFTWARE_CREATION_YEAR "2023"
#define SOFTWARE_REPOSITORY_URL ("https://github.com/skippyr/" SOFTWARE_PACKAGE)
#define SOFTWARE_PACKAGE "libtmk"

static void writeHelp(void);
static void writeVersion(void);
static void writeError(const char *format, ...);
static int isLowerCaseLetter(char byte);
static int isOption(const char *argument);
static int parseArguments(struct tmk_Arguments *arguments);

static char allowsMouseCapture_g = 0;

static void writeHelp(void) {
  tmk_setFontWeight(tmk_Weight_Bold);
  tmk_write("Usage: ");
  tmk_resetFontWeight();
  tmk_write(SOFTWARE_NAME " [");
  tmk_setFontEffects(tmk_Effect_Underline);
  tmk_write("OPTIONS");
  tmk_resetFontEffects();
  tmk_writeLine("]...");
  tmk_writeLine("Logs the bytes of terminal events on macOS and Linux.");
  tmk_writeLine("");
  tmk_setFontWeight(tmk_Weight_Bold);
  tmk_writeLine("AVAILABLE OPTIONS");
  tmk_resetFontWeight();
  tmk_writeLine("  --help             shows the software help instructions.");
  tmk_writeLine("  --version          shows the software version.");
  tmk_writeLine("  --capture-mouse    enables mouse events.");
}

static void writeVersion(void) {
  tmk_setFontWeight(tmk_Weight_Bold);
  tmk_writeLine(SOFTWARE_NAME " " SOFTWARE_VERSION);
  tmk_resetFontWeight();
  tmk_write(SOFTWARE_LICENSE ". Copyright (c) " SOFTWARE_CREATION_YEAR " "
            SOFTWARE_AUTHOR_NAME " <");
  tmk_setFontEffects(tmk_Effect_Underline);
  tmk_write(SOFTWARE_AUTHOR_EMAIL);
  tmk_resetFontEffects();
  tmk_writeLine(">.");
  tmk_writeLine("");
  tmk_write("Its software repository is available at: <");
  tmk_setFontEffects(tmk_Effect_Underline);
  tmk_write(SOFTWARE_REPOSITORY_URL);
  tmk_resetFontEffects();
  tmk_writeLine(">.");
}

static void writeError(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_setFontWeight(tmk_Weight_Bold);
  tmk_writeError(SOFTWARE_NAME ": ");
  tmk_resetFontWeight();
  va_end(arguments);
}

static int isLowerCaseLetter(char byte) {
  return byte >= 'a' && byte <= 'z';
}

static int isOption(const char *argument) {
  size_t length = strlen(argument);
  return (length == 2 && argument[0] == '-' &&
          isLowerCaseLetter(argument[1])) ||
         (length > 2 && argument[0] == '-' && argument[1] == '-' &&
          isLowerCaseLetter(argument[2]));
}

static int parseArguments(struct tmk_Arguments *arguments) {
  for (int offset = 0; offset < arguments->total; ++offset) {
    const char *argument = arguments->asUtf8[offset];
    if (!strcmp(argument, "-h") || !strcmp(argument, "--help")) {
      writeHelp();
      return 1;
    } else if (!strcmp(argument, "-v") || !strcmp(argument, "--version")) {
      writeVersion();
      return 1;
    } else if (!strcmp(argument, "--capture-mouse")) {
      allowsMouseCapture_g = 1;
    } else {
      writeError();
    }
  }
  return 0;
}

int main(int argc, const char **argv) {
  struct tmk_Arguments arguments;
  tmk_getArguments(argc, argv, &arguments);
  parseArguments(&arguments);
  tmk_freeArguments(&arguments);
  return 0;
}
