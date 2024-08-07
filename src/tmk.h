#ifndef _tmk_H
#define _tmk_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(_WIN32)
/**
 * @brief A string that contains the name of the operating system in use.
 */
#define tmk_OPERATING_SYSTEM "Windows"
/**
 * @brief A boolean that states the operating system in use is Windows.
 */
#define tmk_IS_OPERATING_SYSTEM_WINDOWS true
/**
 * @brief A boolean that states the operating system in use is Linux.
 */
#define tmk_IS_OPERATING_SYSTEM_LINUX false
/**
 * @brief A boolean that states the operating system in use is MacOS.
 */
#define tmk_IS_OPERATING_SYSTEM_MACOS false
/**
 * @brief A boolean that states the operating system is unknown.
 */
#define tmk_IS_OPERATING_SYSTEM_UNKNOWN false
#if defined(_M_ARM)
/**
 * @brief A string that contains the name of the CPU architecture in use.
 */
#define tmk_CPU_ARCHITECTURE "ARM32"
/**
 * @brief A boolean that states the CPU architecture is ARM32.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 true
/**
 * @brief A boolean that states the CPU architecture is ARM64.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 false
/**
 * @brief A boolean that states the CPU architecture is x86.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86 false
/**
 * @brief A boolean that states the CPU architecture is x86_64.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 false
/**
 * @brief A boolean that states the CPU architecture is unknown.
 */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN false
#elif defined(_M_ARM64)
/**
 * @brief A string that contains the name of the CPU architecture in use.
 */
#define tmk_CPU_ARCHITECTURE "ARM64"
/**
 * @brief A boolean that states the CPU architecture is ARM32.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 false
/**
 * @brief A boolean that states the CPU architecture is ARM64.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 true
/**
 * @brief A boolean that states the CPU architecture is x86.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86 false
/**
 * @brief A boolean that states the CPU architecture is x86_64.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 false
/**
 * @brief A boolean that states the CPU architecture is unknown.
 */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN false
#elif defined(_WIN64)
/**
 * @brief A string that contains the name of the CPU architecture in use.
 */
#define tmk_CPU_ARCHITECTURE "x86_64"
/**
 * @brief The CPU architecture in use. It is an enumerator from the
 * tmk_CPUArchitecture enum.
 */
/**
 * @brief A boolean that states the CPU architecture is ARM32.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 false
/**
 * @brief A boolean that states the CPU architecture is ARM64.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 false
/**
 * @brief A boolean that states the CPU architecture is x86.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86 false
/**
 * @brief A boolean that states the CPU architecture is x86_64.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 true
/**
 * @brief A boolean that states the CPU architecture is unknown.
 */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN false
#else
/**
 * @brief A string that contains the name of the CPU architecture in use.
 */
#define tmk_CPU_ARCHITECTURE "x86"
/**
 * @brief A boolean that states the CPU architecture is ARM32.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 false
/**
 * @brief A boolean that states the CPU architecture is ARM64.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 false
/**
 * @brief A boolean that states the CPU architecture is x86.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86 true
/**
 * @brief A boolean that states the CPU architecture is x86_64.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 false
/**
 * @brief A boolean that states the CPU architecture is unknown.
 */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN false
#endif
#else
#if defined(__linux__)
/**
 * @brief A string that contains the name of the operating system in use.
 */
#define tmk_OPERATING_SYSTEM "Linux"
/**
 * @brief A boolean that states the operating system in use is Windows.
 */
#define tmk_IS_OPERATING_SYSTEM_WINDOWS false
/**
 * @brief A boolean that states the operating system in use is Linux.
 */
#define tmk_IS_OPERATING_SYSTEM_LINUX true
/**
 * @brief A boolean that states the operating system in use is MacOS.
 */
#define tmk_IS_OPERATING_SYSTEM_MACOS false
/**
 * @brief A boolean that states the operating system is unknown.
 */
#define tmk_IS_OPERATING_SYSTEM_UNKNOWN false
#elif defined(__APPLE__)
/**
 * @brief A string that contains the name of the operating system in use.
 */
#define tmk_OPERATING_SYSTEM "MacOS"
/**
 * @brief A boolean that states the operating system in use is Windows.
 */
#define tmk_IS_OPERATING_SYSTEM_WINDOWS false
/**
 * @brief A boolean that states the operating system in use is Linux.
 */
#define tmk_IS_OPERATING_SYSTEM_LINUX false
/**
 * @brief A boolean that states the operating system in use is MacOS.
 */
#define tmk_IS_OPERATING_SYSTEM_MACOS true
/**
 * @brief A boolean that states the operating system is unknown.
 */
#define tmk_IS_OPERATING_SYSTEM_UNKNOWN false
#else
/**
 * @brief A string that contains the name of the operating system in use.
 */
#define tmk_OPERATING_SYSTEM "Unknown Operating System"
/**
 * @brief A boolean that states the operating system in use is Windows.
 */
#define tmk_IS_OPERATING_SYSTEM_WINDOWS false
/**
 * @brief A boolean that states the operating system in use is Linux.
 */
#define tmk_IS_OPERATING_SYSTEM_LINUX false
/**
 * @brief A boolean that states the operating system in use is MacOS.
 */
#define tmk_IS_OPERATING_SYSTEM_MACOS false
/**
 * @brief A boolean that states the operating system is unknown.
 */
#define tmk_IS_OPERATING_SYSTEM_UNKNOWN true
/**
 * @brief A string that contains the name of the CPU architecture in use.
 */
#define tmk_CPU_ARCHITECTURE "Unknown CPU Architecture"
/**
 * @brief A boolean that states the CPU architecture is ARM32.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 false
/**
 * @brief A boolean that states the CPU architecture is ARM64.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 false
/**
 * @brief A boolean that states the CPU architecture is x86.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86 false
/**
 * @brief A boolean that states the CPU architecture is x86_64.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 false
/**
 * @brief A boolean that states the CPU architecture is unknown.
 */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN true
#endif
#if defined(__linux__) || defined(__APPLE__)
#if defined(__arm__)
/**
 * @brief A string that contains the name of the CPU architecture in use.
 */
#define tmk_CPU_ARCHITECTURE "ARM32"
/**
 * @brief A boolean that states the CPU architecture is ARM32.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 true
/**
 * @brief A boolean that states the CPU architecture is ARM64.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 false
/**
 * @brief A boolean that states the CPU architecture is x86.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86 false
/**
 * @brief A boolean that states the CPU architecture is x86_64.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 false
/**
 * @brief A boolean that states the CPU architecture is unknown.
 */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN false
#elif defined(__aarch64__)
/**
 * @brief A string that contains the name of the CPU architecture in use.
 */
#define tmk_CPU_ARCHITECTURE "ARM64"
/**
 * @brief A boolean that states the CPU architecture is ARM32.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 false
/**
 * @brief A boolean that states the CPU architecture is ARM64.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 true
/**
 * @brief A boolean that states the CPU architecture is x86.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86 false
/**
 * @brief A boolean that states the CPU architecture is x86_64.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 false
/**
 * @brief A boolean that states the CPU architecture is unknown.
 */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN false
#elif defined(__x86_64__)
/**
 * @brief A string that contains the name of the CPU architecture in use.
 */
#define tmk_CPU_ARCHITECTURE "x86_64"
/**
 * @brief A boolean that states the CPU architecture is ARM32.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 false
/**
 * @brief A boolean that states the CPU architecture is ARM64.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 false
/**
 * @brief A boolean that states the CPU architecture is x86.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86 false
/**
 * @brief A boolean that states the CPU architecture is x86_64.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 true
/**
 * @brief A boolean that states the CPU architecture is unknown.
 */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN false
#elif defined(__i386__)
/**
 * @brief A string that contains the name of the CPU architecture in use.
 */
#define tmk_CPU_ARCHITECTURE "x86"
/**
 * @brief A boolean that states the CPU architecture is ARM32.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM32 false
/**
 * @brief A boolean that states the CPU architecture is ARM64.
 */
#define tmk_IS_CPU_ARCHITECTURE_ARM64 false
/**
 * @brief A boolean that states the CPU architecture is x86.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86 true
/**
 * @brief A boolean that states the CPU architecture is x86_64.
 */
#define tmk_IS_CPU_ARCHITECTURE_X86_64 false
/**
 * @brief A boolean that states the CPU architecture is unknown.
 */
#define tmk_IS_CPU_ARCHITECTURE_UNKNOWN false
#endif
#endif
#endif

/**
 * @brief Contains the first 16 ANSI colors of the XTerm color palette. They map
 * to the colors of the current terminal theme. A color may be set by using the
 * tmk_setFontANSIColor function.
 */
enum tmk_ANSIColor {
  /**
   * @brief The dark variant of the black color.
   */
  tmk_ANSIColor_DarkBlack,
  /**
   * @brief The dark variant of the red color.
   */
  tmk_ANSIColor_DarkRed,
  /**
   * @brief The dark variant of the green color.
   */
  tmk_ANSIColor_DarkGreen,
  /**
   * @brief The dark variant of the yellow color.
   */
  tmk_ANSIColor_DarkYellow,
  /**
   * @brief The dark variant of the blue color.
   */
  tmk_ANSIColor_DarkBlue,
  /**
   * @brief The dark variant of the magenta color.
   */
  tmk_ANSIColor_DarkMagenta,
  /**
   * @brief The dark variant of the cyan color.
   */
  tmk_ANSIColor_DarkCyan,
  /**
   * @brief The dark variant of the white color.
   */
  tmk_ANSIColor_DarkWhite,
  /**
   * @brief The light variant of the black color.
   */
  tmk_ANSIColor_LightBlack,
  /**
   * @brief The light variant of the red color.
   */
  tmk_ANSIColor_LightRed,
  /**
   * @brief The light variant of the green color.
   */
  tmk_ANSIColor_LightGreen,
  /**
   * @brief The light variant of the yellow color.
   */
  tmk_ANSIColor_LightYellow,
  /**
   * @brief The light variant of the blue color.
   */
  tmk_ANSIColor_LightBlue,
  /**
   * @brief The light variant of the magenta color.
   */
  tmk_ANSIColor_LightMagenta,
  /**
   * @brief The light variant of the cyan color.
   */
  tmk_ANSIColor_LightCyan,
  /**
   * @brief The light variant of the white color.
   */
  tmk_ANSIColor_LightWhite
};

/**
 * @brief Contains the terminal font layers. They are used to set colors by
 * using the tmk_setFontANSIColor and tmk_setFontRGBColor functions.
 */
enum tmk_FontLayer {
  /**
   * @brief Refers to the graphemes.
   */
  tmk_FontLayer_Foreground = 3,
  /**
   * @brief Refers to the background of the graphemes.
   */
  tmk_FontLayer_Background
};

/**
 * @brief Contains the terminal font weights. A weight may be set by using the
 * tmk_setFontWeight function.
 */
enum tmk_FontWeight {
  /**
   * @brief Usually rendered with bold weight and/or with light colors.
   */
  tmk_FontWeight_Bold = 1,
  /**
   * @brief Usually rendered with faint colors.
   */
  tmk_FontWeight_Light
};

/**
 * @brief Contains the available terminal font effects. One or multiple effects
 * may be set by using the tmk_setFontEffects function.
 */
enum tmk_FontEffect {
  /**
   * @brief Makes the text curly. It may require the use of a font with italic
   * style.
   */
  tmk_FontEffect_Italic = 1 << 3,
  /**
   * @brief Draws a horizontal line under the text.
   */
  tmk_FontEffect_Underline = 1 << 4,
  /**
   * @brief Makes the text blink periodically.
   */
  tmk_FontEffect_Blinking = 1 << 5,
  /**
   * @brief Swaps the foreground and background colors.
   */
  tmk_FontEffect_ReverseVideo = 1 << 7,
  /**
   * @brief Makes the text hard to see or invisible.
   */
  tmk_FontEffect_Conceal = 1 << 8,
  /**
   * @brief Draws a horizontal line in the middle of the text.
   */
  tmk_FontEffect_Strikethrough = 1 << 9
};

/**
 * @brief Contains the terminal cursor shapes. A shape may be set by using the
 * tmk_setCursorShape function.
 */
enum tmk_CursorShape {
  /**
   * @brief Fills the whole cursor region.
   */
  tmk_CursorShape_Block = 2,
  /**
   * @brief Fills a region at the bottom of the cursor region.
   */
  tmk_CursorShape_Underline = 4,
  /**
   * @brief Fills a region at a horizontal side of the cursor region.
   */
  tmk_CursorShape_Bar = 6
};

/**
 * @brief Contains the keyboard keys that are not represented by UTF-8
 * graphemes. They may be set in a tmk_KeyEvent struct by the tmk_readKeyEvent
 * function.
 */
enum tmk_Key {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  /**
   * @brief The left arrow key.
   */
  tmk_Key_LeftArrow = -23,
  /**
   * @brief The up arrow key.
   */
  tmk_Key_UpArrow,
  /**
   * @brief The right arrow key.
   */
  tmk_Key_RightArrow,
  /**
   * @brief The down arrow key.
   */
  tmk_Key_DownArrow,
#else
  /**
   * @brief The up arrow key.
   */
  tmk_Key_UpArrow = -23,
  /**
   * @brief The down arrow key.
   */
  tmk_Key_DownArrow,
  /**
   * @brief The right arrow key.
   */
  tmk_Key_RightArrow,
  /**
   * @brief The left arrow key.
   */
  tmk_Key_LeftArrow,
#endif
  /**
   * @brief The F1 function key.
   */
  tmk_Key_F1,
  /**
   * @brief The F2 function key.
   */
  tmk_Key_F2,
  /**
   * @brief The F3 function key.
   */
  tmk_Key_F3,
  /**
   * @brief The F4 function key.
   */
  tmk_Key_F4,
  /**
   * @brief The F5 function key.
   */
  tmk_Key_F5,
  /**
   * @brief The F6 function key.
   */
  tmk_Key_F6,
  /**
   * @brief The F7 function key.
   */
  tmk_Key_F7,
  /**
   * @brief The F8 function key.
   */
  tmk_Key_F8,
  /**
   * @brief The F9 function key.
   */
  tmk_Key_F9,
  /**
   * @brief The F10 function key.
   */
  tmk_Key_F10,
  /**
   * @brief The F11 function key.
   */
  tmk_Key_F11,
  /**
   * @brief The F12 function key.
   */
  tmk_Key_F12,
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  /**
   * @brief The PageUp key.
   */
  tmk_Key_PageUp,
  /**
   * @brief The PageDown key.
   */
  tmk_Key_PageDown,
  /**
   * @brief The End key.
   */
  tmk_Key_End,
  /**
   * @brief The Home key.
   */
  tmk_Key_Home,
  /**
   * @brief The Insert key.
   */
  tmk_Key_Insert,
  /**
   * @brief The Delete key.
   */
  tmk_Key_Delete,
#else
  /**
   * @brief The Home key.
   */
  tmk_Key_Home,
  /**
   * @brief The Insert key.
   */
  tmk_Key_Insert,
  /**
   * @brief The Delete key.
   */
  tmk_Key_Delete,
  /**
   * @brief The End key.
   */
  tmk_Key_End,
  /**
   * @brief The PageUp key.
   */
  tmk_Key_PageUp,
  /**
   * @brief The PageDown key.
   */
  tmk_Key_PageDown,
#endif
  /**
   * @brief The Tab key.
   */
  tmk_Key_Tab = 9,
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  /**
   * @brief The Enter key.
   */
  tmk_Key_Enter = 13,
#else
  /**
   * @brief The Enter key.
   */
  tmk_Key_Enter,
#endif
  /**
   * @brief The Escape key.
   */
  tmk_Key_Escape = 27,
  /**
   * @brief The Space key.
   */
  tmk_Key_Space = 32,
  /**
   * @brief The backspace key.
   */
  tmk_Key_Backspace = 127
};

/**
 * @brief Contains the keyboard modifier keys. The Shift key may be identified
 * by checking the key read during events. They may be set in a tmk_KeyEvent
 * struct by the tmk_readKeyEvent function.
 */
enum tmk_ModifierKey {
  /**
   * @brief On Windows and Linux, it is the Alt key. On MacOS, the Option key.
   */
  tmk_ModifierKey_AltOption = 1,
  /**
   * @brief The Ctrl key.
   */
  tmk_ModifierKey_Ctrl = 1 << 1,
};

/**
 * @brief Contains the standard terminal streams. Their statuses may be check by
 * using the tmk_isStreamRedirected function.
 */
enum tmk_Stream {
  /**
   * @brief Where events are read from.
   */
  tmk_Stream_Input,
  /**
   * @brief Where regular tense messages are output.
   */
  tmk_Stream_Output,
  /**
   * @brief Where error tense messages are output.
   */
  tmk_Stream_Error
};

/**
 * @brief Represents the command line arguments. It may be filled by using the
 * tmk_getCMDArguments function.
 */
struct tmk_CMDArguments {
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
  /**
   * @brief The arguments in UTF-16 encoding. It is only available on Windows.
   */
  const wchar_t **utf16Arguments;
#endif
  /**
   * @brief The arguments in UTF-8 encoding.
   */
  const char **utf8Arguments;
  /**
   * @brief The total arguments.
   */
  int totalArguments;
};

/**
 * @brief Represents a terminal coordinate. It uses a coordinate system that
 * considers the origin position at the top left corner of the screen, where
 * both column and row components are zero. From there, those components
 * increase going right and down, respectively. It is used by the
 * tmk_getCursorCoordinate and tmk_setCursorCoordinate functions.
 */
struct tmk_Coordinate {
  /**
   * @brief The column component of the coordinate.
   */
  uint16_t column;
  /**
   * @brief The row component of the coordinate.
   */
  uint16_t row;
};

/**
 * @brief Represents terminal dimensions. It is used by the
 * tmk_getWindowDimensions function.
 */
struct tmk_Dimensions {
  /**
   * @brief The area of the dimensions.
   */
  uint32_t area;
  /**
   * @brief The total columns of the dimensions.
   */
  uint16_t totalColumns;
  /**
   * @brief The total rows of the dimensions.
   */
  uint16_t totalRows;
};

/**
 * @brief Represents an RGB color. It may be set by using the
 * tmk_setFontRGBColor function.
 */
struct tmk_RGBColor {
  /**
   * @brief The red component of the color.
   */
  uint8_t red;
  /**
   * @brief The green component of the color.
   */
  uint8_t green;
  /**
   * @brief The blue component of the color.
   */
  uint8_t blue;
};

/**
 * @brief Represents a terminal key event. It may be filled by the
 * tmk_readKeyEvent function.
 */
struct tmk_KeyEvent {
  /**
   * @brief The key pressed. It may be an UTF-8 grapheme or an enumerator from
   * the tmk_Key enum.
   */
  int32_t key;
  /**
   * @brief A bitmask containing the modifier keys being hold during the event.
   * It may be composed by enumerators from the tmk_ModifierKey enum.
   */
  uint8_t modifiers;
};

#if defined(__cplusplus)
extern "C" {
#endif
#if tmk_IS_OPERATING_SYSTEM_WINDOWS
/**
 * @brief Converts an UTF-16 encoded string to UTF-8 encoding. It is only
 * available on Windows.
 * @param utf16String The UTF-16 encoded string.
 * @param length The address where the length of the converted string without
 * including the NULL grapheme at its end will be put into. It is equivalent to
 * the total bytes in the string. It may be freed by using the free function. If
 * not necessary, set it to NULL.
 * @returns The UTF-8 encoded string.
 */
char *tmk_convertUTF16ToUTF8(const wchar_t *utf16String, size_t *length);
/**
 * @brief Converts an UTF-8 encoded string to UTF-16 encoding. It is only
 * available on Windows.
 * @param utf16String The UTF-8 encoded string.
 * @param length The address where the length of the converted string without
 * including the NULL grapheme at its end will be put into. It is equivalent to
 * the total of graphemes, being 2 bytes each, in the string, as that is the
 * measurement unit use by the Windows API UTF-16 functions, even though that
 * encoding supports graphemes up to 4 bytes. It may be freed by using the
 * free function. If not necessary, set it to NULL.
 * @returns The UTF-16 encoded string.
 */
wchar_t *tmk_convertUTF8ToUTF16(const char *utf8String, size_t *length);
#endif
/**
 * @brief Checks if a terminal stream is redirected.
 * @param stream The stream to be checked.
 * @returns A boolean that states the stream is redirected.
 */
bool tmk_isStreamRedirected(enum tmk_Stream stream);
/**
 * @brief Flushes the bytes cached inside of the terminal output buffer.
 */
void tmk_flushOutputBuffer(void);
/**
 * @brief Clears the events cached inside the terminal input buffer.
 */
void tmk_clearInputBuffer(void);
/**
 * @brief Rings the terminal bell, possibly emitting a bell indicator in the
 * title bar, window flash, system notification sound or a beep from the
 * motherboard speaker.
 */
void tmk_ringBell(void);
/**
 * @brief Clears the line the terminal is on.
 */
void tmk_clearCursorLine(void);
/**
 * @brief Gets and converts the command line arguments of the terminal process.
 * It fills a tmk_CMDArguments struct which must be freed by using the
 * tmk_freeCMDArguments function.
 * @param totalRawCMDArguments The total command line arguments given as the
 * first parameter of the main function.
 * @param rawCMDArguments The raw command line arguments given as the second
 * parameter of the main function.
 * @param cmdArguments The address where the command line arguments will be put
 * into.
 */
void tmk_getCMDArguments(int totalRawCMDArguments, const char **rawCMDArguments,
                         struct tmk_CMDArguments *cmdArguments);
/**
 * @brief Frees the memory allocated for the command line arguments of the
 * terminal process.
 */
void tmk_freeCMDArguments(struct tmk_CMDArguments *arguments);
/**
 * @brief Gets the dimensions of the terminal window.
 * @param dimensions The address where the dimensions information will be put
 * into.
 * @returns 0 if successful or -1 otherwise.
 */
int tmk_getWindowDimensions(struct tmk_Dimensions *dimensions);
/**
 * @brief Sets an ANSI color into the terminal font. The color may be reset by
 * using the tmk_resetFontColors function.
 * @param color The color to be applied. It may be a value in range from 0 to
 * 255 or an enumerator from the tmk_ANSIColor enum.
 * @param layer The layer to be affected.
 */
void tmk_setFontANSIColor(uint8_t color, enum tmk_FontLayer layer);
/**
 * @brief Sets an RGB color into the terminal font. The color may be reset by
 * using the tmk_resetFontColors function.
 * @param color The color to be applied.
 * @param layer The layer to be affected.
 */
void tmk_setFontRGBColor(struct tmk_RGBColor color, enum tmk_FontLayer layer);
/**
 * @brief Resets the terminal font foreground and background colors.
 */
void tmk_resetFontColors(void);
/**
 * @brief Sets the terminal font weight. The weight may be reset by using the
 * tmk_resetFontWeight function.
 * @param weight The weight to be set.
 */
void tmk_setFontWeight(enum tmk_FontWeight weight);
/**
 * @brief Resets the terminal font weight.
 */
void tmk_resetFontWeight(void);
/**
 * @brief Sets terminal font effects. Effects may be reset by using the
 * tmk_resetFontEffects function.
 * @param effectsMask A bitmask containing the effects to applied. It must be
 * composed by enumerators from the tmk_FontEffect enum.
 */
void tmk_setFontEffects(int effectsMask);
/**
 * @brief Resets all terminal font effects.
 */
void tmk_resetFontEffects(void);
/**
 * @brief Opens the alternate window. It may be closed by using the
 * tmk_closeAlternateWindow function.
 */
void tmk_openAlternateWindow(void);
/**
 * @brief Closes the alternate window.
 */
void tmk_closeAlternateWindow(void);
/**
 * @brief Gets the terminal cursor coordinate. On Linux and MacOS, as it parses
 * a terminal answer given through the terminal input buffer, it clears its
 * buffer upon execution.
 * @param coordinate The address where the coordinate information will be put
 * into.
 * @returns 0 if successful or -1 otherwise.
 */
int tmk_getCursorCoordinate(struct tmk_Coordinate *coordinate);
/**
 * @brief Sets the terminal cursor coordinate.
 * @param coordinate The coordinate to be set. It will always be fit within the
 * terminal window boundaries.
 */
void tmk_setCursorCoordinate(struct tmk_Coordinate coordinate);
/**
 * @brief Sets the terminal cursor shape. It may be reset by using the
 * tmk_resetCursorShape function.
 * @param shape The shape to be set.
 * @param isBlinking A boolean that states the cursor should blink.
 */
void tmk_setCursorShape(enum tmk_CursorShape shape, bool isBlinking);
/**
 * @brief Resets the terminal cursor shape.
 */
void tmk_resetCursorShape(void);
/**
 * @brief Sets the terminal cursor visibility.
 * @param isVisible A boolean that states the cursor should be visible.
 */
void tmk_setCursorVisibility(bool isVisible);
/**
 * @brief Reads a terminal key event and remove it from the input buffer.
 * Content typed and pasted into the terminal are considered key events. Under
 * some situations, the terminal may not give enough information for the library
 * to distinguish certain key events. Some key sequences may be reserved by the
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
 * @returns 0 if successful; -1 if user interaction was not possible due to
 * stream redirection or wide character orientation; -2 if no events were
 * available; -3 if the timer ran out; Or -4 if the terminal window was resized
 * during the reading.
 */
int tmk_readKeyEvent(int16_t waitInMilliseconds, struct tmk_KeyEvent *event,
                     bool (*filter)(struct tmk_KeyEvent *));
/**
 * @brief Formats and writes a string to the terminal output stream. On Windows,
 * it sets the console output code page to UTF-8.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param arguments The arguments to be formatted.
 */
void tmk_writeArguments(const char *format, va_list arguments);
/**
 * @brief Formats and writes a string to the terminal output stream with a
 * newline grapheme appended to its end. On Windows, it sets the console output
 * code page to UTF-8.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param arguments The arguments to be formatted.
 */
void tmk_writeArgumentsLine(const char *format, va_list arguments);
/**
 * @brief Formats and writes a string to the terminal error stream. On Windows,
 * it sets the console output code page to UTF-8. In order to synchronize
 * possible ANSI escape sequences cached, it flushes the terminal output buffer
 * upon execution.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param arguments The arguments to be formatted.
 */
void tmk_writeErrorArguments(const char *format, va_list arguments);
/**
 * @brief Formats and writes a string to the terminal error stream with a
 * newline grapheme appended to its end. On Windows, it sets the console output
 * code page to UTF-8. In order to synchronize possible ANSI escape sequences
 * cached, it flushes the terminal output buffer upon execution.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param arguments The arguments to be formatted.
 */
void tmk_writeErrorArgumentsLine(const char *format, va_list arguments);
/**
 * @brief Formats and writes a string to the terminal output stream. On Windows,
 * it sets the console output code page to UTF-8.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param ... The arguments to be formatted.
 */
void tmk_write(const char *format, ...);
/**
 * @brief Formats and writes a string to the terminal output stream with a
 * newline grapheme appended to its end. On Windows, it sets the console output
 * code page to UTF-8.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param ... The arguments to be formatted.
 */
void tmk_writeLine(const char *format, ...);
/**
 * @brief Formats and writes a string to the terminal error stream. On Windows,
 * it sets the console output code page to UTF-8. In order to synchronize
 * possible ANSI escape sequences cached, it flushes the terminal output buffer
 * upon execution.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param ... The arguments to be formatted.
 */
void tmk_writeError(const char *format, ...);
/**
 * @brief Formats and writes a string to the terminal error stream with a
 * newline grapheme appended to its end. On Windows, it sets the console output
 * code page to UTF-8. In order to synchronize possible ANSI escape sequences
 * cached, it flushes the terminal output buffer upon execution.
 * @param format The format to be used. It accepts the same format specifiers as
 * the printf function family.
 * @param ... The arguments to be formatted.
 */
void tmk_writeErrorLine(const char *format, ...);
#if defined(__cplusplus)
}
#endif
#endif
