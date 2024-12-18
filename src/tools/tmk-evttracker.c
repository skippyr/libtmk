#include <tmk.h>
#include <string.h>

#define SOFTWARE_NAME "tmk-evttracker"
#define SOFTWARE_VERSION "v1.0.0"

static void writeError(const char *format, ...);
static void writeHelp(void);
static void writeVersion(void);
static int isLowerCaseLetter(char byte);
static int isOption(const char *argument);

static void writeError(const char *format, ...) {
	va_list arguments;
	va_start(arguments, format);
	tmk_writeError("%s ", SOFTWARE_NAME);
	tmk_setFontAnsiColor(tmk_AnsiColor_LightBlack, tmk_Layer_Foreground);
	tmk_write("(exit code 1)");
	tmk_resetFontColors();
	tmk_write(": ");
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
	tmk_writeLine("%s %s", SOFTWARE_NAME, SOFTWARE_VERSION);
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
	tmk_writeLine("Tracking events...");
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
