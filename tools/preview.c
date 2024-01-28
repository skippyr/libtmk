#include <string.h>
#include <tdk.h>

static void bottombar(void);
static void desktop(void);
static void die(char *msg);
static void topbar(void);
static void win(void);

static int wcol, wln;

static void
bottombar(void)
{
	tdk_setcpos(wcol - 18, wln - 2);
	printf("  skippyr/libtdk");
	tdk_setcpos(wcol - 30, wln - 1);
	tdk_setclr(tdk_ClrRed, tdk_LyrFg);
	tdk_seteff(tdk_EffUnd, 1);
	printf("www.github.com/skippyr/libtdk");
	tdk_seteff(tdk_EffUnd, 0);
	tdk_setclr(tdk_ClrDft, tdk_LyrFg);
	tdk_setcpos(0, wln - 1);
	printf("   Press ");
	tdk_setlum(tdk_LumBld);
	printf("Escape");
	tdk_setlum(tdk_LumDft);
	printf(" or ");
	tdk_setlum(tdk_LumBld);
	printf("Q");
	tdk_setlum(tdk_LumDft);
	printf(" to exit preview.");
}

static void
desktop(void)
{
	tdk_setcpos(0, 2);
	printf("     \n");
	printf("  Trash\n\n");
	printf("     \n");
	printf(" Firefox\n\n");
	printf("     \n");
	printf(" Help.pdf\n\n");
}

static void
die(char *msg)
{
	tdk_setclr(tdk_ClrRed, tdk_LyrFg);
	fprintf(stderr, "preview: ");
	tdk_setclr(tdk_ClrDft, tdk_LyrFg);
	fprintf(stderr, "%s", msg);
	exit(1);
}

static void
topbar(void)
{
	int i;
	tdk_seteff(tdk_EffRev, 1);
	for (i = 0; i < wcol; i++)
		printf(" ");
	tdk_setcpos(wcol - 37, 0);
	printf("󰃭 (Wed) Jan 24th  󰽥 19h00m   Search");
	tdk_setcpos(0, 0);
	tdk_setlum(tdk_LumBld);
	printf("   libtdk - preview ");
	tdk_setlum(tdk_LumDft);
	printf(" File  View  Options  Help");
	tdk_seteff(tdk_EffRev, 0);
}

static void
win(void)
{
	tdk_setcpos(wcol - 30, 5);
	tdk_seteff(tdk_EffRev, 1);
	printf(" ┌ 󰀦  Alert ─────[x]─┐ ");
	tdk_setcpos(wcol - 30, 6);
	printf(" │ Here Are Dragons! │ ");
	tdk_setcpos(wcol - 30, 7);
	printf(" └───────────────────┘ ");
	tdk_seteff(tdk_EffRev, 0);
}

int
main(void)
{
	int key;
	tdk_getwdim(&wcol, &wln);
	if (!isatty(1) || !isatty(2))
		die("no output stream must be redirected.\n");
	if (wcol < 90 || wln < 17)
		die("your terminal must have, at least, 90 col x 17 ln.\n");
	tdk_setwalt(1);
	tdk_setcvis(0);
	topbar();
	bottombar();
	desktop();
	win();
	while ((key = tdk_getkey()) != tdk_KeyEsc && key != 'q' && key != 'Q');
	tdk_setcvis(1);
	tdk_setwalt(0);
	return 0;
}
