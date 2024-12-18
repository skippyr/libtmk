#include <tmk.h>

#define SOFTWARE_NAME "tmk-evttracker"
#define SOFTWARE_VERSION "v1.0.0"

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
	tmk_writeLine("    --help             shows the software help instructions.");
	tmk_writeLine("    --version          shows the software version.");
	tmk_writeLine("    --capture-mouse    allows the capture of mouse events.");
}

int main(int totalMainArguments, const char **mainArguments) {
	struct tmk_CommandLineArguments commandLineArguments;
	tmk_getCommandLineArguments(totalMainArguments, mainArguments, &commandLineArguments);
	tmk_freeCommandLineArguments(&commandLineArguments);
	return 0;
}
