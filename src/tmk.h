#pragma once

#include <stdarg.h>
#include <stdlib.h>

#if defined(_WIN32)
/** A string that contains the name of the operating system in use. */
#define tmk_OPERATING_SYSTEM "Windows"
/** A boolean that states the operating system in use is Windows. */
#define tmk_IS_OPERATING_SYSTEM_WINDOWS 1
/** A boolean that states the operating system in use is Linux. */
#define tmk_IS_OPERATING_SYSTEM_LINUX 0
/** A boolean that states the operating system in use is MacOS. */
#define tmk_IS_OPERATING_SYSTEM_MACOS 0
/** A boolean that states the operating system is unknown. */
#define tmk_IS_OPERATING_SYSTEM_UNKNOWN 0
#if defined(_M_ARM)
/** A string that contains the name of the CPU architecture in use. */
#define tmk_CPU_ARCHITECTURE "ARM32"
/** A boolean that states the CPU architecture is ARM32. */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 1
/** A boolean that states the CPU architecture is ARM64. */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 0
/** A boolean that states the CPU architecture is x86. */
#define tmk_IS_CPU_ARCHITECTURE_X86 0
/** A boolean that states the CPU architecture is x86_64. */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 0
/** A boolean that states the CPU architecture is unknown. */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN 0
#elif defined(_M_ARM64)
/** A string that contains the name of the CPU architecture in use. */
#define tmk_CPU_ARCHITECTURE "ARM64"
/** A boolean that states the CPU architecture is ARM32. */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 0
/** A boolean that states the CPU architecture is ARM64. */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 1
/** A boolean that states the CPU architecture is x86. */
#define tmk_IS_CPU_ARCHITECTURE_X86 0
/** A boolean that states the CPU architecture is x86_64. */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 0
/** A boolean that states the CPU architecture is unknown. */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN 0
#elif defined(_WIN64)
/** A string that contains the name of the CPU architecture in use. */
#define tmk_CPU_ARCHITECTURE "x86_64"
/** A boolean that states the CPU architecture is ARM32. */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 0
/** A boolean that states the CPU architecture is ARM64. */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 0
/** A boolean that states the CPU architecture is x86. */
#define tmk_IS_CPU_ARCHITECTURE_X86 0
/** A boolean that states the CPU architecture is x86_64. */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 1
/** A boolean that states the CPU architecture is unknown. */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN 0
#else
/** A string that contains the name of the CPU architecture in use. */
#define tmk_CPU_ARCHITECTURE "x86"
/** A boolean that states the CPU architecture is ARM32. */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 0
/** A boolean that states the CPU architecture is ARM64. */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 0
/** A boolean that states the CPU architecture is x86. */
#define tmk_IS_CPU_ARCHITECTURE_X86 1
/** A boolean that states the CPU architecture is x86_64. */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 0
/** A boolean that states the CPU architecture is unknown. */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN 0
#endif
#else
#if defined(__linux__)
/** A string that contains the name of the operating system in use. */
#define tmk_OPERATING_SYSTEM "Linux"
/** A boolean that states the operating system in use is Windows. */
#define tmk_IS_OPERATING_SYSTEM_WINDOWS 0
/** A boolean that states the operating system in use is Linux. */
#define tmk_IS_OPERATING_SYSTEM_LINUX 1
/** A boolean that states the operating system in use is MacOS. */
#define tmk_IS_OPERATING_SYSTEM_MACOS 0
/** A boolean that states the operating system is unknown. */
#define tmk_IS_OPERATING_SYSTEM_UNKNOWN 0
#elif defined(__APPLE__)
/** A string that contains the name of the operating system in use. */
#define tmk_OPERATING_SYSTEM "MacOS"
/** A boolean that states the operating system in use is Windows. */
#define tmk_IS_OPERATING_SYSTEM_WINDOWS 0
/** A boolean that states the operating system in use is Linux. */
#define tmk_IS_OPERATING_SYSTEM_LINUX 0
/** A boolean that states the operating system in use is MacOS. */
#define tmk_IS_OPERATING_SYSTEM_MACOS 1
/** A boolean that states the operating system is unknown. */
#define tmk_IS_OPERATING_SYSTEM_UNKNOWN 0
#else
/** A string that contains the name of the operating system in use. */
#define tmk_OPERATING_SYSTEM "Unknown Operating System"
/** A boolean that states the operating system in use is Windows. */
#define tmk_IS_OPERATING_SYSTEM_WINDOWS 0
/** A boolean that states the operating system in use is Linux. */
#define tmk_IS_OPERATING_SYSTEM_LINUX 0
/** A boolean that states the operating system in use is MacOS. */
#define tmk_IS_OPERATING_SYSTEM_MACOS 0
/** A boolean that states the operating system is unknown. */
#define tmk_IS_OPERATING_SYSTEM_UNKNOWN 1
/** A string that contains the name of the CPU architecture in use. */
#define tmk_CPU_ARCHITECTURE "Unknown CPU Architecture"
/** A boolean that states the CPU architecture is ARM32. */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 0
/** A boolean that states the CPU architecture is ARM64. */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 0
/** A boolean that states the CPU architecture is x86. */
#define tmk_IS_CPU_ARCHITECTURE_X86 0
/** A boolean that states the CPU architecture is x86_64. */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 0
/** A boolean that states the CPU architecture is unknown. */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN 1
#endif
#if defined(__linux__) || defined(__APPLE__)
#if defined(__arm__)
/** A string that contains the name of the CPU architecture in use. */
#define tmk_CPU_ARCHITECTURE "ARM32"
/** A boolean that states the CPU architecture is ARM32. */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 1
/** A boolean that states the CPU architecture is ARM64. */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 0
/** A boolean that states the CPU architecture is x86. */
#define tmk_IS_CPU_ARCHITECTURE_X86 0
/** A boolean that states the CPU architecture is x86_64. */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 0
/** A boolean that states the CPU architecture is unknown. */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN 0
#elif defined(__aarch64__)
/** A string that contains the name of the CPU architecture in use. */
#define tmk_CPU_ARCHITECTURE "ARM64"
/** A boolean that states the CPU architecture is ARM32. */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 0
/** A boolean that states the CPU architecture is ARM64. */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 1
/** A boolean that states the CPU architecture is x86. */
#define tmk_IS_CPU_ARCHITECTURE_X86 0
/** A boolean that states the CPU architecture is x86_64. */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 0
/** A boolean that states the CPU architecture is unknown. */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN 0
#elif defined(__x86_64__)
/** A string that contains the name of the CPU architecture in use. */
#define tmk_CPU_ARCHITECTURE "x86_64"
/** A boolean that states the CPU architecture is ARM32. */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 0
/** A boolean that states the CPU architecture is ARM64. */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 0
/** A boolean that states the CPU architecture is x86. */
#define tmk_IS_CPU_ARCHITECTURE_X86 0
/** A boolean that states the CPU architecture is x86_64. */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 1
/** A boolean that states the CPU architecture is unknown. */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN 0
#elif defined(__i386__)
/** A string that contains the name of the CPU architecture in use. */
#define tmk_CPU_ARCHITECTURE "x86"
/** A boolean that states the CPU architecture is ARM32. */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 0
/** A boolean that states the CPU architecture is ARM64. */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 0
/** A boolean that states the CPU architecture is x86. */
#define tmk_IS_CPU_ARCHITECTURE_X86 1
/** A boolean that states the CPU architecture is x86_64. */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 0
/** A boolean that states the CPU architecture is unknown. */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN 0
#endif
#endif
#endif
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
/** The minimum exit code accepted by the operating system. */
#define tmk_MINIMUM_EXIT_CODE -2147483647
/** The minimum exit code accepted by the operating system. */
#define tmk_MAXIMUM_EXIT_CODE 2147483647
#else
/** The minimum exit code accepted by the operating system. */
#define tmk_MINIMUM_EXIT_CODE 0
/** The minimum exit code accepted by the operating system. */
#define tmk_MAXIMUM_EXIT_CODE 255
#endif

/**
 * Contains the first 16 ANSI colors of the XTerm color palette. They map to the
 * colors of the current terminal theme. A color may be set by using the
 * tmk_setFontAnsiColor function.
 */
enum tmk_AnsiColor {
  /** The dark variant of the black color. */
  tmk_AnsiColor_DarkBlack,
  /** The dark variant of the red color. */
  tmk_AnsiColor_DarkRed,
  /** The dark variant of the green color. */
  tmk_AnsiColor_DarkGreen,
  /** The dark variant of the yellow color. */
  tmk_AnsiColor_DarkYellow,
  /** The dark variant of the blue color. */
  tmk_AnsiColor_DarkBlue,
  /** The dark variant of the magenta color. */
  tmk_AnsiColor_DarkMagenta,
  /** The dark variant of the cyan color. */
  tmk_AnsiColor_DarkCyan,
  /** The dark variant of the white color. */
  tmk_AnsiColor_DarkWhite,
  /** The light variant of the black color. */
  tmk_AnsiColor_LightBlack,
  /** The light variant of the red color. */
  tmk_AnsiColor_LightRed,
  /** The light variant of the green color. */
  tmk_AnsiColor_LightGreen,
  /** The light variant of the yellow color. */
  tmk_AnsiColor_LightYellow,
  /** The light variant of the blue color. */
  tmk_AnsiColor_LightBlue,
  /** The light variant of the magenta color. */
  tmk_AnsiColor_LightMagenta,
  /** The light variant of the cyan color. */
  tmk_AnsiColor_LightCyan,
  /** The light variant of the white color. */
  tmk_AnsiColor_LightWhite
};

/** Contains the terminal layers. They are used as reference to set colors when
 * using the tmk_setFontAnsiColor and tmk_setFontRgbColor functions. */
enum tmk_Layer {
  /** Refers to the front layer, where the graphemes are. */
  tmk_Layer_Foreground = 3,
  /** Refers to the background layer, behind the graphemes. */
  tmk_Layer_Background
};

/** Contains the terminal font weights. A weight may be set by using the
 * tmk_setFontWeight function. */
enum tmk_FontWeight {
  /** Usually rendered with bold weight and/or with light colors. */
  tmk_FontWeight_Bold = 1,
  /** Usually rendered with faint colors. */
  tmk_FontWeight_Light
};

/** Contains the available terminal font effects. One or multiple effects may be
 * set by using the tmk_setFontEffects function. */
enum tmk_FontEffect {
  /** Makes the text curly. It may require the use of a font with italic style.
   */
  tmk_FontEffect_Italic = 1 << 3,
  /** Draws a horizontal line under the text. */
  tmk_FontEffect_Underline = 1 << 4,
  /** Makes the text blink periodically. */
  tmk_FontEffect_Blinking = 1 << 5,
  /** Swaps the foreground and background colors. */
  tmk_FontEffect_ReverseVideo = 1 << 7,
  /** Makes the text hard to see or invisible. */
  tmk_FontEffect_Conceal = 1 << 8,
  /** Draws a horizontal line in the middle of the text. */
  tmk_FontEffect_Strikethrough = 1 << 9
};

/** Contains the terminal cursor shapes. A shape may be set by using the
 * tmk_setCursorShape function. */
enum tmk_CursorShape {
  /** Fills the whole cursor region. */
  tmk_CursorShape_Block = 2,
  /** Fills a region at the bottom of the cursor region. */
  tmk_CursorShape_Underline = 4,
  /** Fills a region at a horizontal side of the cursor region. */
  tmk_CursorShape_Bar = 6
};

/** Contains the keyboard keys that are not represented by UTF-8 graphemes. They
 * may be set in a tmk_KeyEvent struct by the tmk_readKeyEvent function. */
enum tmk_Key {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  /** The Left Arrow key. */
  tmk_Key_LeftArrow = -23,
  /** The Up Arrow key. */
  tmk_Key_UpArrow,
  /** The Right Arrow key. */
  tmk_Key_RightArrow,
  /** The Down Arrow key. */
  tmk_Key_DownArrow,
#else
  /** The Up Arrow key. */
  tmk_Key_UpArrow = -23,
  /** The Down Arrow key. */
  tmk_Key_DownArrow,
  /** The Right Arrow key. */
  tmk_Key_RightArrow,
  /** The Left Arrow key. */
  tmk_Key_LeftArrow,
#endif
  /** The F1 function key. */
  tmk_Key_F1,
  /** The F2 function key. */
  tmk_Key_F2,
  /** The F3 function key. */
  tmk_Key_F3,
  /** The F4 function key. */
  tmk_Key_F4,
  /** The F5 function key. */
  tmk_Key_F5,
  /** The F6 function key. */
  tmk_Key_F6,
  /** The F7 function key. */
  tmk_Key_F7,
  /** The F8 function key. */
  tmk_Key_F8,
  /** The F9 function key. */
  tmk_Key_F9,
  /** The F10 function key. */
  tmk_Key_F10,
  /** The F11 function key. */
  tmk_Key_F11,
  /** The F12 function key. */
  tmk_Key_F12,
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  /** The Page Up key. */
  tmk_Key_PageUp,
  /** The Page Down key. */
  tmk_Key_PageDown,
  /** The End key. */
  tmk_Key_End,
  /** The Home key. */
  tmk_Key_Home,
  /** The Insert key. */
  tmk_Key_Insert,
  /** The Delete key. */
  tmk_Key_Delete,
#else
  /** The Home key. */
  tmk_Key_Home,
  /** The Insert key. */
  tmk_Key_Insert,
  /** The Delete key. */
  tmk_Key_Delete,
  /** The End key. */
  tmk_Key_End,
  /** The Page Up key. */
  tmk_Key_PageUp,
  /** The Page Down key. */
  tmk_Key_PageDown,
#endif
  /** The Tab key. */
  tmk_Key_Tab = 9,
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  /** The Enter key. */
  tmk_Key_Enter = 13,
#else
  /** The Enter key. */
  tmk_Key_Enter,
#endif
  /** The Escape key. */
  tmk_Key_Escape = 27,
  /** The Spacebar key. */
  tmk_Key_Spacebar = 32,
  /** The Backspace key. */
  tmk_Key_Backspace = 127
};

/**
 * Contains the keyboard modifier keys. The Shift key may be identified by
 * checking the key read during events. They may be set in a tmk_KeyEvent struct
 * by the tmk_readKeyEvent function.
 */
enum tmk_ModifierKey {
  /** On Windows and Linux, it is the Alt key. On MacOS, the Option key. */
  tmk_ModifierKey_AltOrOption = 1,
  /** The Ctrl key. */
  tmk_ModifierKey_Ctrl = 1 << 1,
};

/** Contains the standard terminal streams. Their statuses may be check by using
 * the tmk_isStreamRedirected function. */
enum tmk_Stream {
  /** Where events are read from. */
  tmk_Stream_Input,
  /** Where regular tense messages are output. */
  tmk_Stream_Output,
  /** Where error tense messages are output. */
  tmk_Stream_Error
};

/** Represents the command line arguments. It may be filled by using the
 * tmk_getCmdArguments function. */
struct tmk_CommandLineArguments {
  /** The total arguments. */
  int totalArguments;
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  /** The arguments in UTF-16 encoding. It is only available on Windows. */
  const wchar_t **utf16Arguments;
#endif
  /** The arguments in UTF-8 encoding. */
  const char **utf8Arguments;
};

/**
 * Represents a terminal coordinate. It uses a coordinate system that considers
 * the origin position at the top left corner of the screen, where both column
 * and row components are zero. From there, those components increase going
 * right and down, respectively. It is used by the tmk_getCursorCoordinate and
 * tmk_setCursorCoordinate functions.
 */
struct tmk_Coordinate {
  /** The column component of the coordinate. */
  unsigned short column;
  /** The row component of the coordinate. */
  unsigned short row;
};

/** Represents terminal dimensions. It is used by the tmk_getWindowDimensions
 * function. */
struct tmk_WindowDimensions {
  /** The total columns of the dimensions. */
  unsigned short totalColumns;
  /** The total rows of the dimensions. */
  unsigned short totalRows;
  /** The area of the dimensions. */
  unsigned area;
};

/** Represents an RGB color. It may be set by using the tmk_setFontRgbColor
 * function. */
struct tmk_RgbColor {
  /** The red component of the color. */
  unsigned char red;
  /** The green component of the color. */
  unsigned char green;
  /** The blue component of the color. */
  unsigned char blue;
};

/** Represents a terminal key event. It may be filled by the tmk_readKeyEvent
 * function. */
struct tmk_KeyEvent {
  /** A bitmask containing the modifier keys being hold during the event. It may
   * be composed by enumerators from the tmk_ModifierKey enum. */
  unsigned char modifiers;
  /** The key pressed. It may be an UTF-8 grapheme or an enumerator from the
   * tmk_Key enum. */
  int key;
};

#if defined(__cplusplus)
extern "C" {
#endif
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
/**
 * Converts an UTF-16 encoded string to UTF-8 encoding. It is only available on
 * Windows.
 * @param utf16String The UTF-16 encoded string.
 * @param length The address where the length of the converted string without
 * including the NULL grapheme at its end will be put into. It is equivalent to
 * the total bytes in the string. It may be freed by using the free function. If
 * not necessary, set it to NULL.
 * @return The UTF-8 encoded string.
 */
char *tmk_convertUtf16ToUtf8(const wchar_t *utf16String, size_t *length);
/**
 * Converts an UTF-8 encoded string to UTF-16 encoding. It is only available on
 * Windows.
 * @param utf16String The UTF-8 encoded string.
 * @param length The address where the length of the converted string without
 * including the NULL grapheme at its end will be put into. It is equivalent to
 * the total of graphemes, being 2 bytes each, in the string, as that is the
 * measurement unit use by the Windows API UTF-16 functions, even though that
 * encoding supports graphemes up to 4 bytes. It may be freed by using the free
 * function. If not necessary, set it to NULL.
 * @return The UTF-16 encoded string.
 */
wchar_t *tmk_convertUtf8ToUtf16(const char *utf8String, size_t *length);
#endif
/**
 * Checks if a terminal stream is redirected.
 * @param stream The stream to be checked.
 * @return A boolean that states the stream is redirected.
 */
int tmk_isStreamRedirected(enum tmk_Stream stream);
/** Flushes the bytes cached inside of the terminal output buffer. */
void tmk_flushOutputBuffer(void);
/** Clears the events cached inside the terminal input buffer. */
void tmk_clearInputBuffer(void);
/** Rings the terminal bell, possibly emitting a bell indicator in the title
 * bar, window flash, system notification sound or a beep from the motherboard
 * speaker. */
void tmk_ringBell(void);
/** Clears the line the terminal is on. */
void tmk_clearCursorLine(void);
/**
 * Gets and converts the command line arguments of the terminal process. It
 * fills a tmk_CommandLineArguments struct which must be freed by using the
 * tmk_freeCmdArguments function.
 * @param totalMainArguments The total command line arguments given as the first
 * parameter of the main function.
 * @param mainArguments The raw command line arguments given as the second
 * parameter of the main function.
 * @param arguments The address where the command line arguments will be put
 * into.
 */
void tmk_getCommandLineArguments(int totalMainArguments,
                                 const char **mainArguments,
                                 struct tmk_CommandLineArguments *arguments);
/** Frees the memory allocated for the command line arguments of the terminal
 * process. */
void tmk_freeCommandLineArguments(struct tmk_CommandLineArguments *arguments);
/**
 * Gets the dimensions of the terminal window.
 * @param dimensions The address where the dimensions information will be put
 * into.
 * @return 0 if successful or -1 otherwise.
 */
int tmk_getWindowDimensions(struct tmk_WindowDimensions *dimensions);
/**
 * Sets an ANSI color into the terminal font. The color may be reset by using
 * the tmk_resetFontColors function.
 * @param color The color to be applied. It may be a value in range from 0 to
 * 255 or an enumerator from the tmk_AnsiColor enum.
 * @param layer The layer to be affected.
 */
void tmk_setFontAnsiColor(unsigned char color, enum tmk_Layer layer);
/**
 * Sets an RGB color into the terminal font. The color may be reset by using the
 * tmk_resetFontColors function.
 * @param color The color to be applied.
 * @param layer The layer to be affected.
 */
void tmk_setFontRgbColor(struct tmk_RgbColor color, enum tmk_Layer layer);
/** Resets the terminal font foreground and background colors. */
void tmk_resetFontColors(void);
/**
 * Sets the terminal font weight. The weight may be reset by using the
 * tmk_resetFontWeight function.
 * @param weight The weight to be set.
 */
void tmk_setFontWeight(enum tmk_FontWeight weight);
/** Resets the terminal font weight. */
void tmk_resetFontWeight(void);
/**
 * Sets terminal font effects. Effects may be reset by using the
 * tmk_resetFontEffects function.
 * @param effectsMask A bitmask containing the effects to applied. It must be
 * composed by enumerators from the tmk_FontEffect enum.
 */
void tmk_setFontEffects(int effectsMask);
/** Resets all terminal font effects. */
void tmk_resetFontEffects(void);
/** Opens the alternate window. It may be closed by using the
 * tmk_closeAlternateWindow function. On Windows, it causes a window resize
 * event. */
void tmk_openAlternateWindow(void);
/** Closes the alternate window. */
void tmk_closeAlternateWindow(void);
/**
 * Gets the terminal cursor coordinate. On Linux and MacOS, as it parses a
 * terminal answer given through the terminal input buffer, it clears its buffer
 * upon execution.
 * @param coordinate The address where the coordinate information will be put
 * into.
 * @return 0 if successful or -1 otherwise.
 */
int tmk_getCursorCoordinate(struct tmk_Coordinate *coordinate);
/**
 * Sets the terminal cursor coordinate.
 * @param coordinate The coordinate to be set. It will always be fit within the
 * terminal window boundaries.
 */
void tmk_setCursorCoordinate(struct tmk_Coordinate coordinate);
/**
 * Sets the terminal cursor shape. It may be reset by using the
 * tmk_resetCursorShape function.
 * @param shape The shape to be set.
 * @param shouldBlink A boolean that states the cursor should blink.
 */
void tmk_setCursorShape(enum tmk_CursorShape shape, int shouldBlink);
/** Resets the terminal cursor shape. */
void tmk_resetCursorShape(void);
/**
 * Sets the terminal cursor visibility.
 * @param isVisible A boolean that states the cursor should be visible.
 */
void tmk_setCursorVisible(int isVisible);
/**
 * Reads a terminal key event and remove it from the input buffer. Content typed
 * and pasted into the terminal are considered key events. Under some
 * situations, the terminal may not give enough information for the library to
 * distinguish certain key events. Some key sequences may be reserved by the
 * terminal or operating system. Modifiers keys may only be identified in keys
 * represented by UTF-8 graphemes. During execution, it blocks the receipt of
 * signals. For convenience, it flushes the terminal output buffer upon
 * execution. To avoid reading cached events, use it with the
 * tmk_clearInputBuffer function.
 * @param waitInMilliseconds The time in milliseconds to wait for an event. If
 * the time is negative, it waits forever; If zero, it returns immediately; Or,
 * if positive, it waits the given time.
 * @param event The address where the event information will be put into. If not
 * necessary, it may be set to NULL.
 * @param filter a function to filter events while the timer is running,
 * avoiding to destroy it unless necessary. It returns a boolean that states the
 * event should be considered. It is only used when the wait specified is
 * positive. If not necessary, set it to NULL.
 * @return 0 if successful; -1 if user interaction was not possible due to
 * stream redirection or wide character orientation; -2 if no events were
 * available; -3 if the timer ran out; Or -4 if the terminal window was resized
 * during the reading.
 */
int tmk_readKeyEvent(short waitInMilliseconds, struct tmk_KeyEvent *event,
                     int (*filter)(struct tmk_KeyEvent *));
/**
 * Formats and writes a string to the terminal output stream. On Windows, it
 * sets the console output code page to UTF-8.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param arguments The arguments to be formatted.
 */
void tmk_writeArguments(const char *format, va_list arguments);
/**
 * Formats and writes a string to the terminal output stream with a newline
 * grapheme appended to its end. On Windows, it sets the console output code
 * page to UTF-8.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param arguments The arguments to be formatted.
 */
void tmk_writeArgumentsLine(const char *format, va_list arguments);
/**
 * Formats and writes a string to the terminal error stream. On Windows, it sets
 * the console output code page to UTF-8. In order to synchronize possible ANSI
 * escape sequences cached, it flushes the terminal output buffer upon
 * execution.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param arguments The arguments to be formatted.
 */
void tmk_writeErrorArguments(const char *format, va_list arguments);
/**
 * Formats and writes a string to the terminal error stream with a newline
 * grapheme appended to its end. On Windows, it sets the console output code
 * page to UTF-8. In order to synchronize possible ANSI escape sequences cached,
 * it flushes the terminal output buffer upon execution.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param arguments The arguments to be formatted.
 */
void tmk_writeErrorArgumentsLine(const char *format, va_list arguments);
/**
 * Formats and writes a string to the terminal output stream. On Windows, it
 * sets the console output code page to UTF-8.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param ... The arguments to be formatted.
 */
void tmk_write(const char *format, ...);
/**
 * Formats and writes a string to the terminal output stream with a newline
 * grapheme appended to its end. On Windows, it sets the console output code
 * page to UTF-8.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param ... The arguments to be formatted.
 */
void tmk_writeLine(const char *format, ...);
/**
 * Formats and writes a string to the terminal error stream. On Windows, it sets
 * the console output code page to UTF-8. In order to synchronize possible ANSI
 * escape sequences cached, it flushes the terminal output buffer upon
 * execution.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param ... The arguments to be formatted.
 */
void tmk_writeError(const char *format, ...);
/**
 * Formats and writes a string to the terminal error stream with a newline
 * grapheme appended to its end. On Windows, it sets the console output code
 * page to UTF-8. In order to synchronize possible ANSI escape sequences cached,
 * it flushes the terminal output buffer upon execution.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param ... The arguments to be formatted.
 */
void tmk_writeErrorLine(const char *format, ...);
#if defined(__cplusplus)
}
#endif
