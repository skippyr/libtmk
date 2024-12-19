#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <tmk.h>
#include <unistd.h>

#define SOFTWARE_NAME "tmk-event-tracker"
#define SOFTWARE_VERSION "v1.0.0"
#define SOFTWARE_AUTHOR "Sherman Rofeman"
#define SOFTWARE_AUTHOR_EMAIL "skippyr.developer@icloud.com"
#define SOFTWARE_AUTHOR_NICKNAME "skippyr"
#define SOFTWARE_REPOSITORY_URL "https://github.com/" SOFTWARE_AUTHOR_NICKNAME "/" SOFTWARE_PACKAGE
#define SOFTWARE_CREATION_YEAR "2024"
#define SOFTWARE_LICENSE "BSD-3-Clause License"
#define SOFTWARE_PACKAGE "libtmk"
#define RAW_MODE_C_LFLAGS (ICANON | ECHO | ISIG)
#define RAW_MODE_C_IFLAGS (IXON)

static void writeError(const char *format, ...);
static void writeHelp(void);
static void writeVersion(void);
static int isLowerCaseLetter(char byte);
static int isOption(const char *argument);
static void enableRawMode(void);
static void disableRawMode(void);
static void enableBlockingInput(void);
static void disableBlockingInput(void);
static void trackEvents(void);

static int allowsMouseCapture_g = 0;

static void writeError(const char *format, ...) {
	va_list arguments;
	va_start(arguments, format);
	tmk_setFontWeight(tmk_Weight_Bold);
	tmk_writeError("%s: ", SOFTWARE_NAME);
	tmk_resetFontWeight();
	tmk_writeErrorArguments(format, arguments);
	tmk_writeErrorLine(" Use -h or --help for help instructions.");
	va_end(arguments);
}

static void writeHelp(void) {
	tmk_setFontWeight(tmk_Weight_Bold);
	tmk_write("Usage: ");
	tmk_resetFontWeight();
	tmk_write("%s [", SOFTWARE_NAME);
	tmk_setFontEffects(tmk_Effect_Underline);
	tmk_write("OPTIONS");
	tmk_resetFontEffects();
	tmk_writeLine("]...");
	tmk_writeLine("Describes the terminal events that happens during its execution.");
	tmk_writeLine("");
	tmk_setFontWeight(tmk_Weight_Bold);
	tmk_writeLine("AVAILABLE OPTIONS");
	tmk_resetFontWeight();
	tmk_writeLine("    -h, --help         shows the software help instructions.");
	tmk_writeLine("    -v, --version      shows the software version.");
	tmk_writeLine("    --capture-mouse    allows the capture of mouse events.");
}

static void writeVersion(void) {
	tmk_setFontWeight(tmk_Weight_Bold);
	tmk_write(SOFTWARE_NAME);
	tmk_resetFontWeight();
	tmk_writeLine(" %s", SOFTWARE_VERSION);
	tmk_write("%s. Copyright (c) %s %s <", SOFTWARE_LICENSE, SOFTWARE_CREATION_YEAR, SOFTWARE_AUTHOR);
	tmk_setFontEffects(tmk_Effect_Underline);
	tmk_write(SOFTWARE_AUTHOR_EMAIL);
	tmk_resetFontEffects();
	tmk_writeLine(">.");
	tmk_writeLine("");
	tmk_write("Software repository available at: <");
	tmk_setFontEffects(tmk_Effect_Underline);
	tmk_write(SOFTWARE_REPOSITORY_URL);
	tmk_resetFontEffects();
	tmk_writeLine(">.");
	tmk_writeLine("Distributed as part of %s.", SOFTWARE_PACKAGE);
}

static int parseCommandLineArguments(struct tmk_CommandLineArguments *commandLineArguments) {
	for (int offset = 1; offset < commandLineArguments->total; ++offset) {
		const char *argument = commandLineArguments->asUtf8[offset];
		if (!strcmp(argument, "-h") || !strcmp(argument, "--help")) {
			writeHelp();
			return 1;
		} else if (!strcmp(argument, "-v") || !strcmp(argument, "--version")) {
			writeVersion();
			return 1;
		} else if (!strcmp(argument, "--allow-mouse-capture")) {
			allowsMouseCapture_g = 1;
		} else {
			writeError(isOption(argument) ? "unrecognized option \"%s\"." : "malformed option \"%s\".", argument);
			return -1;
		}
	}
	return 0;
}

static int isLowerCaseLetter(char byte) {
	return byte >= 'a' && byte <= 'z';
}

static int isOption(const char *argument) {
	size_t length = strlen(argument);
	return (length == 2 && argument[0] == '-' && isLowerCaseLetter(argument[1])) || (length > 2 && argument[0] == '-' && argument[1] == '-' && isLowerCaseLetter(argument[2]));
}

static void enableRawMode(void) {
	struct termios attributes;
	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag &= ~RAW_MODE_C_LFLAGS;
	attributes.c_iflag &= ~RAW_MODE_C_IFLAGS;
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
}

static void disableRawMode(void) {
	struct termios attributes;
	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag |= RAW_MODE_C_LFLAGS;
	attributes.c_iflag |= RAW_MODE_C_IFLAGS;
	tcsetattr(STDIN_FILENO, TCSANOW, &attributes);
}

static void enableBlockingInput(void) {
	int flags = fcntl(STDIN_FILENO, F_GETFL);
	fcntl(STDIN_FILENO, F_SETFL, flags &~ O_NONBLOCK);
}

static void disableBlockingInput(void) {
	int flags = fcntl(STDIN_FILENO, F_GETFL);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

static void trackEvents(void) {
	tmk_write("Press the ");
	tmk_setFontWeight(tmk_Weight_Bold);
	tmk_write("[Esc]");
	tmk_resetFontWeight();
	tmk_writeLine(" key to exit the program.");
	tmk_writeLine("Awaiting for terminal events to log...");
	tmk_writeLine("");
	int offset = 0;
	tmk_setCursorShape(tmk_Shape_Underline, 0);
	tmk_setCursorVisible(0);
	for (; 1; ++offset) {
		struct tmk_Coordinate coordinate;
		tmk_setFontWeight(tmk_Weight_Bold);
		tmk_write("Event %d: ", offset);
		tmk_resetFontWeight();
		tmk_getCursorCoordinate(&coordinate);
		tmk_setFontAnsiColor(tmk_AnsiColor_LightBlack, tmk_Layer_Foreground);
		tmk_write("<<awaiting bytes>> ");
		tmk_resetFontColors();
		tmk_setCursorVisible(1);
		enableRawMode();
		int byte = getchar();
		tmk_setCursorVisible(0);
		tmk_setCursorCoordinate(coordinate);
		tmk_clearUntilEndOfLine();
		tmk_write("%d", byte);
		disableBlockingInput();
		if (byte == 27) {
			if ((byte = getchar()) == EOF) {
				break;
			}
			tmk_write(" %d", byte);
		}
		while ((byte = getchar()) != EOF) {
			tmk_write(" %d", byte);
		}
		tmk_writeLine("");
		enableBlockingInput();
	}
	tmk_setCursorVisible(1);
	tmk_resetCursorShape();
	tmk_writeLine("");
	tmk_writeLine("");
	tmk_writeLine("Exited after read %d %s.", offset, offset == 1 ? "event" : "events");
	enableBlockingInput();
	disableRawMode();
}

int main(int totalMainArguments, const char **mainArguments) {
	struct tmk_CommandLineArguments commandLineArguments;
	tmk_getCommandLineArguments(totalMainArguments, mainArguments, &commandLineArguments);
	int parseResult = parseCommandLineArguments(&commandLineArguments);
	tmk_freeCommandLineArguments(&commandLineArguments);
	if (!parseResult) {
		trackEvents();
	}
	return parseResult < 0;
}
