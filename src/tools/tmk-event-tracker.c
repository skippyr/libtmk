#include <tmk.h>
#if !defined(_WIN32)
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#endif

#define SOFTWARE_NAME "tmk-event-tracker"
#define SOFTWARE_VERSION "v1.0.0"
#define SOFTWARE_AUTHOR "Sherman Rofeman"
#define SOFTWARE_AUTHOR_EMAIL "skippyr.developer@icloud.com"
#define SOFTWARE_AUTHOR_NICKNAME "skippyr"
#define SOFTWARE_REPOSITORY_URL "https://github.com/" SOFTWARE_AUTHOR_NICKNAME "/" SOFTWARE_PACKAGE
#define SOFTWARE_CREATION_YEAR "2024"
#define SOFTWARE_LICENSE "BSD-3-Clause License"
#define SOFTWARE_PACKAGE "libtmk"
#if !defined(_WIN32)
#define RAW_MODE_C_LFLAGS (ICANON | ECHO | ISIG)
#define RAW_MODE_C_IFLAGS (IXON)
#endif

#if !defined(_WIN32)
static void enableRawMode(void);
static void disableRawMode(void);
static void enableBlockingInput(void);
static void disableBlockingInput(void);
static void enableFocusCapture(void);
static void disableFocusCapture(void);
#endif
static void enableMouseCapture(void);
static void disableMouseCapture(void);
static void writeError(const char *format, ...);
static void writeHelp(void);
static void writeVersion(void);
static int isLowerCaseLetter(char byte);
static int isOption(const char *argument);
static void trackEvents(void);

static int allowsMouseCapture_g = 0;

#if !defined(_WIN32)
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

static void enableFocusCapture(void) {
	tmk_write("\x1b[?1004h");
}

static void disableFocusCapture(void) {
	tmk_write("\x1b[?1004l");
}
#endif

static void enableMouseCapture(void) {
#if defined(_WIN32)
#else
	tmk_write("\x1b[?1003;1015h");
#endif
}

static void disableMouseCapture(void) {
#if defined(_WIN32)
#else
	tmk_write("\x1b[?1003;1015l");
#endif
}

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
		} else if (!strcmp(argument, "--capture-mouse")) {
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

static void trackEvents(void) {
	tmk_write("Press the ");
	tmk_setFontWeight(tmk_Weight_Bold);
	tmk_write("[Esc]");
	tmk_resetFontWeight();
	tmk_writeLine(" key to exit the program.");
	tmk_writeLine("Awaiting for terminal events to log...");
	tmk_writeLine("");
#if defined(_WIN32)
#else
	enableFocusCapture();
	if (allowsMouseCapture_g) {
		enableMouseCapture();
	}
	int index = 1;
	tmk_setCursorShape(tmk_Shape_Underline, 0);
	tmk_setCursorVisible(0);
	for (; 1; ++index) {
		struct tmk_Coordinate coordinate;
		tmk_setFontWeight(tmk_Weight_Bold);
		tmk_write("Event %d: ", index);
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
	tmk_writeLine("Exited after read %d %s.", index, index == 1 ? "event" : "events");
	enableBlockingInput();
	disableRawMode();
	disableFocusCapture();
	if (allowsMouseCapture_g) {
		disableMouseCapture();
	}
#endif
}

int main(int totalMainArguments, const char **mainArguments) {
	if (tmk_isStreamRedirected(tmk_Stream_Input) || tmk_isStreamRedirected(tmk_Stream_Output) || tmk_isStreamRedirected(tmk_Stream_Error)) {
		writeError("no stream must be redirected.");
		return 1;
	}
	struct tmk_CommandLineArguments commandLineArguments;
	tmk_getCommandLineArguments(totalMainArguments, mainArguments, &commandLineArguments);
	int parseResult = parseCommandLineArguments(&commandLineArguments);
	tmk_freeCommandLineArguments(&commandLineArguments);
	if (!parseResult) {
		trackEvents();
	}
	return parseResult < 0;
}
