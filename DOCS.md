<p align="center">
  <img alt="" src="assets/ornament.png" width=1020 />
</p>
<h1 align="center">≥v≥v&ensp;libtmk&ensp;≥v≥v</h1>
<p align="center">Terminal Manipulation Kit - Documentation - v13.0.0</p>

> [!TIP]
> Use GitHub's Outline feature available at the top right corner of this view and your browser Find feature to navigate throughout this document easily. For offline reading, you can use your browser Print function to download this page as a PDF.

## ❡ Reserved Names

This library reserves all names starting with `tmk` and `_tmk`. The underscore at the start indicates that that name is reserved for the library implementation.

## ❡ Included Headers

When included, the following headers are forwarded:

```c
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
```

## ❡ Limitations

This library has some limitations over its usage. To avoid conflicts and malfunctions, you must follow these rules:

- It uses byte oriented functions from the standard library for UTF-8 encoding support. Thus, all standard streams must be kept unoriented or under that same orientation.
- It does not provide full resets of terminal properties. Avoid manipulating them manually or by using functions from other libraries.
- It is not prepared for multithreading. Always use only one thread to manipulate the terminal and avoid resources racing.

## ❡ Macros

- `tmk_OPERATING_SYSTEM`: A string that contains the name of the operating system in use. Possible values are: `"Windows"`, `"Linux"`, `"MacOS"` and `"Unknown Operating System"`.
- `tmk_IS_OPERATING_SYSTEM_WINDOWS`: A boolean that states the operating system in use is Windows.
- `tmk_IS_OPERATING_SYSTEM_LINUX`: A boolean that states the operating system in use is Linux.
- `tmk_IS_OPERATING_SYSTEM_MACOS`: A boolean that states the operating system in use is MacOS.
- `tmk_IS_OPERATING_SYSTEM_UNKNOWN`: A boolean that states the operating system in use is unknown.
- `tmk_CPU_ARCHITECTURE`: A string that contains the name of the CPU architecture in use. Possible values are: `"ARM32"`, `"ARM64"`, `"x86"`, `"x86_64"` and `"Unknown CPU Architecture"`.
- `tmk_IS_CPU_ARCHITECTURE_ARM32`: A boolean that states the CPU architecture in use is ARM32.
- `tmk_IS_CPU_ARCHITECTURE_ARM64`: A boolean that states the CPU architecture in use is ARM64.
- `tmk_IS_CPU_ARCHITECTURE_X86`: A boolean that states the CPU architecture in use is x86.
- `tmk_IS_CPU_ARCHITECTURE_X86_64`: A boolean that states the CPU architecture in use is x86_64.
- `tmk_IS_CPU_ARCHITECTURE_UNKNOWN`: A boolean that states the CPU architecture in use is unknown.

## ❡ Enums

### tmk_ANSIColor Enum

#### Brief

Contains the first 16 ANSI colors of the XTerm color palette. They map to the colors of the current terminal theme. A color may be set by using the [`tmk_setFontANSIColor`](#tmk_setfontansicolor-function) function.

#### Enumerators

- `tmk_ANSIColor_DarkBlack`: the dark variant of the black color.
- `tmk_ANSIColor_DarkRed`: the dark variant of the red color.
- `tmk_ANSIColor_DarkGreen`: the dark variant of the green color.
- `tmk_ANSIColor_DarkYellow`: the dark variant of the yellow color.
- `tmk_ANSIColor_DarkBlue`: the dark variant of the blue color.
- `tmk_ANSIColor_DarkMagenta`: the dark variant of the magenta color.
- `tmk_ANSIColor_DarkCyan`: the dark variant of the cyan color.
- `tmk_ANSIColor_DarkWhite`: the dark variant of the white color.
- `tmk_ANSIColor_LightBlack`: the light variant of the black color.
- `tmk_ANSIColor_LightRed`: the light variant of the red color.
- `tmk_ANSIColor_LightGreen`: the light variant of the green color.
- `tmk_ANSIColor_LightYellow`: the light variant of the yellow color.
- `tmk_ANSIColor_LightBlue`: the light variant of the blue color.
- `tmk_ANSIColor_LightMagenta`: the light variant of the magenta color.
- `tmk_ANSIColor_LightCyan`: the light variant of the cyan color.
- `tmk_ANSIColor_LightWhite`: the light variant of the white color.

### tmk_FontLayer Enum

#### Brief

Contains the terminal font layers. They are used to set colors by using the [`tmk_setFontANSIColor`](#tmk_setfontansicolor-function) and [`tmk_setFontRGBColor`](#tmk_setfontrgbcolor-function) functions.

#### Enumerators

- `tmk_FontLayer_Foreground`: refers to the graphemes.
- `tmk_FontLayer_Background`: refers to the background of the graphemes.

### tmk_FontWeight Enum

#### Brief

Contains the terminal font weights. A weight may be set by using the [`tmk_setFontWeight`](#tmk_setfontweight-function) function.

#### Enumerators

- `tmk_FontWeight_Bold`: usually rendered with bold weight and/or with light colors.
- `tmk_FontWeight_Light`: usually rendered with faint colors.

### tmk_FontEffect Enum

#### Brief

Contains the available terminal effects. One or multiple effects may be set by using the [`tmk_setFontEffects`](#tmk_setfonteffects-function) function.

#### Enumerators

- `tmk_FontEffect_Italic`: makes the text curly. It may require the use of a font with italic style.
- `tmk_FontEffect_Underline`: draws a horizontal line under the text.
- `tmk_FontEffect_Blinking`: makes the text blink periodically.
- `tmk_FontEffect_ReverseVideo`: swaps the foreground and background colors.
- `tmk_FontEffect_Conceal`: makes the text hard to see or invisible.
- `tmk_FontEffect_Strikethrough`: draws a horizontal line in the middle of the text.

### tmk_CursorShape Enum

#### Brief

Contains the terminal cursor shapes. A shape may be set by using the [`tmk_setCursorShape`](#tmk_setcursorshape-function) function.

#### Fields

- `tmk_CursorShape_Block`: fills the whole cursor region.
- `tmk_CursorShape_Underline`: fills a region at the bottom of the cursor region.
- `tmk_CursorShape_Bar`: fills a region at a horizontal side of the cursor region.

### tmk_Key Enum

#### Brief

Contains the keyboard keys that are not represented by UTF-8 graphemes. They may be set in a [`tmk_KeyEvent`](#tmk_keyevent-struct) struct by the [`tmk_readKeyEvent`](#tmk_readkeyevent-function) function.

#### Enumerators

- `tmk_Key_UpArrow`: the up arrow key.
- `tmk_Key_DownArrow`: the down arrow key.
- `tmk_Key_RightArrow`: the right arrow key.
- `tmk_Key_LeftArrow`: the left arrow key.
- `tmk_Key_F1`: the F1 function key.
- `tmk_Key_F2`: the F2 function key.
- `tmk_Key_F3`: the F3 function key.
- `tmk_Key_F4`: the F4 function key.
- `tmk_Key_F5`: the F5 function key.
- `tmk_Key_F6`: the F6 function key.
- `tmk_Key_F7`: the F7 function key.
- `tmk_Key_F8`: the F8 function key.
- `tmk_Key_F9`: the F9 function key.
- `tmk_Key_F10`: the F10 function key.
- `tmk_Key_F11`: the F11 function key.
- `tmk_Key_F12`: the F12 function key.
- `tmk_Key_Home`: the Home key.
- `tmk_Key_Insert`: the Insert key.
- `tmk_Key_Delete`: the Delete key.
- `tmk_Key_End`: the End key.
- `tmk_Key_PageUp`: the PageUp key.
- `tmk_Key_PageDown`: the PageDown key.
- `tmk_Key_Tab`: the Tab key.
- `tmk_Key_Enter`: the Enter key.
- `tmk_Key_Escape`: the Escape key.
- `tmk_Key_Space`: the Space key.
- `tmk_Key_Backspace`: the Backspace key.

### tmk_ModifierKey Enum

#### Brief

Contains the keyboard modifier keys. The Shift key may be identified by checking the key read during events. They may be set in a [`tmk_KeyEvent`](#tmk_keyevent-struct) struct by the [`tmk_readKeyEvent`](#tmk_readkeyevent-function) function.

#### Enumerators

- `tmk_ModifierKey_AltOption`: On Windows and Linux, it is the Alt key. On MacOS, the Option key.
- `tmk_ModifierKey_Ctrl`: The Ctrl key.

### tmk_Stream Enum

#### Brief

Contains the standard terminal streams. Their statuses may be checked by using the [`tmk_isStreamRedirected`](#tmk_isstreamredirected-function) function.

#### Enumerators

- `tmk_Stream_Input`: where events are read from.
- `tmk_Stream_Output`: where regular tense messages are output.
- `tmk_Stream_Error`: where error tense messages are output.

## ❡ Structs

### tmk_CMDArguments Struct

#### Brief

Represents the command line arguments. It may be filled by using the [`tmk_getCMDArguments`](#tmk_getcmdarguments-function) function.

#### Declaration

```c
struct tmk_CMDArguments {
#if defined(_WIN32)
  const wchar_t **utf16Arguments;
#endif
  const char **utf8Arguments;
  int totalArguments;
};
```

#### Fields

- `utf16Arguments`: The arguments in UTF-16 encoding. It is only available on Windows.
- `utf8Arguments`: the arguments in UTF-8 encoding.
- `totalArguments`: the total arguments.

### tmk_Coordinate Struct

#### Brief

Represents a terminal coordinate. It uses a coordinate system that considers the origin position at the top left corner of the screen, where both column and row components are zero. From there, those components increase going right and down, respectively. It is used by the [`tmk_getCursorCoordinate`](#tmk_getcursorcoordinate-function) and [`tmk_setCursorCoordinate`](#tmk_setcursorcoordinate-function) functions.

#### Declaration

```c
struct tmk_Coordinate {
  uint16_t column;
  uint16_t row;
};
```

#### Fields

- `column`: the column component of the coordinate.
- `row`: the row component of the coordinate.

### tmk_Dimensions Struct

#### Brief

Represents terminal dimensions. It is used by the [`tmk_getWindowDimensions`](#tmk_getwindowdimensions-function) function.

#### Declaration

```c
struct tmk_Dimensions {
  uint32_t area;
  uint16_t totalColumns;
  uint16_t totalRows;
};
```

#### Fields

- `area`: the area of the dimensions.
- `totalColumns`: the total of columns of the dimensions.
- `totalRows`: the total of rows of the dimensions.

### tmk_RGBColor Struct

#### Brief

Represents an RGB color. It may be set by using the [`tmk_setFontRGBColor`](#tmk_setfontrgbcolor-function) function.

#### Declaration

```c
struct tmk_RGBColor {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};
```

#### Fields

- `red`: the red component of the color.
- `green`: the green component of the color.
- `blue`: the blue component of the color.

### tmk_KeyEvent Struct

#### Brief

Represents a terminal key event. It may be filled by the [`tmk_readKeyEvent`](#tmk_readkeyevent-function) function.

#### Declaration

```c
struct tmk_KeyEvent {
  int32_t key;
  uint8_t modifiers;
};
```

#### Fields

- `key`: the key pressed. It may be an UTF-8 grapheme or an enumerator from the [`tmk_Key`](#tmk_key-enum) enum.
- `modifiers`: A bitmask containing the modifier keys being hold during the event. It may be composed by enumerators from the [`tmk_ModifierKey`](#tmk_modifierkey-enum) enum.

## ❡ Functions

### tmk_convertUTF16ToUTF8 Function

#### Declaration

```c
char *tmk_convertUTF16ToUTF8(const wchar_t *utf16String, size_t *length);
```

#### Brief

Converts an UTF-16 encoded string to UTF-8 encoding. It is only available on Windows.

#### Parameters

- `utf16String`: the UTF-16 encoded string.
- `length`: the address where the length of the converted string without including the NULL grapheme at its end will be put into. It is equivalent to the total bytes in the string. It may be freed by using the `free` function. If not necessary, set it to `NULL`.

#### Return Value

The UTF-8 encoded string.

#### Example

```c
#include <Windows.h>
#include <tmk.h>

int main(void) {
  size_t length;
  char *message = tmk_convertUTF16ToUTF8(L"Here Be Dragons!", &length);
  tmk_writeLine(":: String: %s", message);
  tmk_writeLine(":: Length: %zu.", length);
  free(message);
  return 0;
}
```

### tmk_convertUTF8ToUTF16 Function

#### Declaration

```c
wchar_t *tmk_convertUTF8ToUTF16(const char *utf8String, size_t *length);
```

#### Brief

Converts an UTF-8 encoded string to UTF-16 encoding. It is only available on Windows.

#### Parameters

- `utf8String`: the UTF-8 encoded string.
- `length`: the address where the length of the converted string without including the NULL grapheme at its end will be put into. It is equivalent to the total of graphemes, being 2 bytes each, in the string, as that is the measurement unit use by the Windows API UTF-16 functions, even though that encoding supports graphemes up to 4 bytes. It may be freed by using the `free` function. If not necessary, set it to `NULL`.

#### Return Value

The UTF-16 encoded string.

#### Example

```c
#include <Windows.h>
#include <tmk.h>

int main(void) {
  size_t length;
  wchar_t *message = tmk_convertUTF8ToUTF16("Here Be Dragons!", &length);
  tmk_write(":: String: ");
  WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), message, length, NULL, NULL);
  tmk_writeLine("");
  tmk_writeLine(":: Length: %zu.", length);
  free(message);
  return 0;
}
```

### tmk_isStreamRedirected Function

#### Declaration

```c
bool tmk_isStreamRedirected(enum tmk_Stream stream);
```

#### Brief

Checks if a terminal stream is redirected.

#### Parameters

- `stream`: the stream to be checked.

#### Return Value

A boolean that states the stream is redirected.

#### Example

```c
#include <tmk.h>

#define BOOLEAN_STRING(value_a) (value_a ? "True" : "False")

int main(void) {
  tmk_writeLine("Stream Redirection Statuses");
  tmk_writeLine("::  Input: %s.",
                BOOLEAN_STRING(tmk_isStreamRedirected(tmk_Stream_Input)));
  tmk_writeLine(":: Output: %s.",
                BOOLEAN_STRING(tmk_isStreamRedirected(tmk_Stream_Output)));
  tmk_writeLine("::  Error: %s.",
                BOOLEAN_STRING(tmk_isStreamRedirected(tmk_Stream_Error)));
  return 0;
}
```

### tmk_flushOutputBuffer Function

#### Brief

Flushes the bytes cached inside of the terminal output buffer.

#### Declaration

```c
void tmk_flushOutputBuffer(void);
```

#### Example

```c
/* Example only for Linux and MacOS. */

#include <tmk.h>
#include <unistd.h>

int main(void) {
  tmk_write("Here Be Dragons!");
  tmk_flushOutputBuffer();
  sleep(1);
  return 0;
}
```

### tmk_clearInputBuffer Function

#### Brief

Clears the events cached inside the terminal input buffer.

#### Declaration

```c
void tmk_clearInputBuffer(void);
```

#### Example

See the example from the [`tmk_readKeyEvent`](#tmk_readkeyevent-function) function.

### tmk_ringBell Function

#### Brief

Rings the terminal bell, possibly emitting a bell indicator in the title bar, window flash, system notification sound or a beep from the motherboard speaker.

#### Declaration

```c
int tmk_ringBell(int stream);
```

#### Example

```c
#include <tmk.h>

int main(void) {
  tmk_ringBell();
  return 0;
}
```

### tmk_clearCursorLine Function

#### Brief

Clears the line the terminal is on.

#### Example

```c
#include <tmk.h>

int main(void) {
  tmk_write("Hello, world!");
  tmk_clearCursorLine();
  tmk_writeLine("Here Be Dragons!");
  return 0;
}
```

### tmk_getCMDArguments Function

#### Declaration

```c
void tmk_getCMDArguments(int totalRawCMDArguments, const char **rawCMDArguments,
                         struct tmk_CMDArguments *cmdArguments);
```

#### Brief

Gets and converts the command line arguments of the terminal process. It fills a [`tmk_CMDArguments`](#tmk_cmdarguments-struct) struct which must be freed by using the [`tmk_freeCMDArguments`](#tmk_freecmdarguments-function) function.

#### Parameters

- `totalRawCMDArguments`: the total command line arguments given as the first parameter of the main function.
- `rawCMDArguments`: the raw command line arguments given as the second parameter of the main function.
- `cmdArguments`: the address where the command line arguments will be put into.

#### Example

```c
#include <tmk.h>

int main(int totalRawCMDArguments, const char **rawCMDArguments) {
  struct tmk_CMDArguments cmdArguments;
  tmk_getCMDArguments(totalRawCMDArguments, rawCMDArguments, &cmdArguments);
  for (int offset = 0; offset < cmdArguments.totalArguments; ++offset) {
    tmk_writeLine("Argument %d: %s.", offset,
                  cmdArguments.utf8Arguments[offset]);
  }
  tmk_freeCMDArguments(&cmdArguments);
}
```

### tmk_freeCMDArguments Function

#### Brief

Frees the memory allocated for the command line arguments of the terminal process.

#### Declaration

```c
void tmk_freeCMDArguments(struct tmk_CMDArguments *arguments);
```

#### Example

See the example from the [`tmk_getCMDArguments`](#tmk_getcmdarguments-function) function.

### tmk_getWindowDimensions Function

#### Brief

Gets the dimensions of the terminal window.

#### Declaration

```c
int tmk_getWindowDimensions(struct tmk_Dimensions *dimensions);
```

#### Parameters

- `dimensions`: the address where the dimensions information will be put into.

#### Return Value

0 if successful or -1 otherwise.

#### Example

```c
#include <tmk.h>

int main(void) {
  struct tmk_Dimensions windowDimensions;
  tmk_getWindowDimensions(&windowDimensions);
  tmk_writeLine(":: Total Columns: %hu.", windowDimensions.totalColumns);
  tmk_writeLine("::    Total Rows: %hu.", windowDimensions.totalRows);
  tmk_writeLine("::          Area: %u.", windowDimensions.area);
  return 0;
}
```

### tmk_setFontANSIColor Function

#### Brief

Sets an ANSI color into the terminal font. The color may be reset by using the [`tmk_resetFontColors`](#tmk_resetfontcolors) function.

#### Declaration

```c
void tmk_setFontANSIColor(uint8_t color, enum tmk_FontLayer layer);
```

#### Parameters

- `color`: the color to be applied. It may be a value in range from 0 to 255 or an enumerator from the [`tmk_ANSIColor`](#tmk_ansicolor-enum) enum.
- `layer`: the layer to be affected.

#### Example

```c
#include <tmk.h>

int main(void) {
  for (int color = 0; color < 255; ++color) {
    tmk_setFontANSIColor(color, tmk_FontLayer_Background);
    tmk_write("   ");
    tmk_resetFontColors();
  }
  tmk_writeLine("");
  tmk_writeLine("");
  tmk_setFontANSIColor(tmk_ANSIColor_DarkRed, tmk_FontLayer_Foreground);
  tmk_writeLine("Here Be Dragons!");
  tmk_resetFontColors();
  tmk_setFontANSIColor(tmk_ANSIColor_LightYellow, tmk_FontLayer_Background);
  tmk_setFontANSIColor(tmk_ANSIColor_DarkBlack, tmk_FontLayer_Foreground);
  tmk_writeLine("Here Be Dragons!");
  tmk_resetFontColors();
  tmk_writeLine("Here Be Dragons!");
}
```

### tmk_setFontRGBColor Function

#### Brief

Sets an RGB color into the terminal font. The color may be reset by using the [`tmk_resetFontColors`](#tmk_resetfontcolors) function.

#### Declaration

```c
void tmk_setFontRGBColor(struct tmk_RGBColor color, enum tmk_FontLayer layer);
```

#### Parameters

- `color`: the color to be applied.
- `layer`: the layer to be affected.

#### Example

```c
#include <tmk.h>

int main(void) {
  /* See http://github.com/skippyr/flamerial for the full palette. */
  struct tmk_RGBColor palette[] = {{179, 29, 19},  {153, 1, 1},   {199, 104, 0},
                                  {176, 133, 72}, {138, 56, 32}, {156, 57, 0},
                                  {233, 211, 151}};
  for (int offset = 0; offset < sizeof(palette) / sizeof(palette[0]); ++offset) {
    tmk_setFontRGBColor(palette[offset], tmk_FontLayer_Background);
    tmk_write("   ");
    tmk_resetFontColors();
    tmk_setFontRGBColor(palette[offset], tmk_FontLayer_Foreground);
    tmk_writeLine(" Here Be Dragons!");
    tmk_resetFontColors();
  }
}
```

### tmk_resetFontColors Function

#### Brief

Resets the terminal font foreground and background colors.

#### Declaration

```c
void tmk_resetFontColors(void);
```

#### Example

See the examples from the [`tmk_setFontANSIColor`](#tmk_setfontansicolor-function) and [`tmk_setFontRGBColor`](#tmk_setfontrgbcolor-function) functions.

### tmk_setFontWeight Function

#### Brief

Sets the terminal font weight. The weight may be reset by using the [`tmk_resetFontWeight`](#tmk_resetfontweight) function.

#### Declaration

```c
void tmk_setFontWeight(enum tmk_FontWeight weight);
```

#### Parameters

- `weight`: the weight to be set.

#### Example

```c
#include <tmk.h>

int main(void) {
  tmk_setFontWeight(tmk_FontWeight_Bold);
  tmk_writeLine("Here Be Dragons!");
  tmk_setFontWeight(tmk_FontWeight_Light);
  tmk_writeLine("Here Be Dragons!");
  tmk_resetFontWeight();
  tmk_writeLine("Here Be Dragons!");
  return 0;
}
```

### tmk_resetFontWeight Function

#### Brief

Resets the terminal font weight.

#### Declaration

```c
void tmk_resetFontWeight(void);
```

#### Example

See the example from the [`tmk_setFontWeight`](#tmk_setfontweight-function) function.

### tmk_setFontEffects Function

#### Brief

Sets terminal font effects. Effects may be reset by using the [`tmk_resetFontEffects`](#tmk_resetfonteffects-function) function.

#### Declaration

```c
void tmk_setFontEffects(int effectsMask);
```

#### Parameters

- `effectsMask`: a bitmask containing the effects to be applied. It must be composed by enumerators from the [`tmk_FontEffect`](#tmk_fonteffect-enum) enum.

#### Example

```c
#include <tmk.h>

int main(void) {
  tmk_setFontEffects(tmk_FontEffect_Underline | tmk_FontEffect_ReverseVideo);
  tmk_writeLine("Here Be Dragons!");
  tmk_resetFontEffects();
  tmk_writeLine("Here Be Dragons!");
  return 0;
}
```

### tmk_resetFontEffects Function

#### Brief

Resets all terminal font effects.

#### Declaration

```c
void tmk_resetFontEffects(void);
```

#### Example

See the example from the [`tmk_setFontEffects`](#tmk_setfonteffects-function) function.

### tmk_openAlternateWindow Function

#### Brief

Opens the alternate window. It may closed by using the [`tmk_closeAlternateWindow`](#tmk_closealternatewindow-function) function.

#### Declaration

```c
void tmk_openAlternateWindow(void);
```

#### Example

```c
#include <tmk.h>

int main(void) {
  tmk_openAlternateWindow();
  tmk_writeLine("Here Be Dragons!");
  tmk_readKeyEvent(-1, NULL, NULL);
  tmk_closeAlternateWindow();
  return 0;
}
```

### tmk_closeAlternateWindow Function

#### Brief

Closes the alternate window.

#### Declaration

```c
void tmk_closeAlternateWindow(void);
```

#### Example

See the example from the [`tmk_openAlternateWindow`](#tmk_openalternatewindow-function) function.

### tmk_getCursorCoordinate Function

#### Brief

Gets the terminal cursor coordinate. On Linux and MacOS, as it parses a terminal answer given through the terminal input buffer, it clears its buffer upon execution.

#### Declaration

```c
int tmk_getCursorCoordinate(struct tmk_Coordinate* coordinate);
```

#### Parameters

- `coordinate`: the address where the coordinate information will be put into.

#### Return Value

0 if successful or -1 otherwise.

#### Example

```c
#include <tmk.h>

int main(void) {
  struct tmk_Coordinate cursorCoordinate;
  tmk_getCursorCoordinate(&cursorCoordinate);
  tmk_writeLine(":: Cursor Column: %hu.", cursorCoordinate.column);
  tmk_writeLine(":: Cursor Row   : %hu.", cursorCoordinate.row);
  return 0;
}
```

### tmk_setCursorCoordinate Function

#### Brief

Sets the terminal cursor coordinate.

#### Declaration

```c
void tmk_setCursorCoordinate(struct tmk_Coordinate coordinate);
```

#### Parameters

- `coordinate`: the coordinate to be set. It will always be fit within the terminal window boundaries.

#### Example

```c
#include <tmk.h>

int main(void) {
  struct tmk_Coordinate cursorCoordinate;
  tmk_getCursorCoordinate(&cursorCoordinate);
  cursorCoordinate.column = 5;
  tmk_setCursorCoordinate(cursorCoordinate);
  tmk_writeLine("Here Be Dragons!");
  return 0;
}
```

### tmk_setCursorShape Function

#### Brief

Sets the terminal cursor shape. It may be reset by using the [`tmk_resetCursorShape`](#tmk_resetcursorshape-function) function.

#### Declaration

```c
void tmk_setCursorShape(enum tmk_CursorShape shape, bool isBlinking);
```

#### Parameters

- `shape`: the shape to be set.
- `isBlinking`: a boolean that states the cursor should blink.

#### Example

```c
#include <tmk.h>

int main(void) {
  tmk_setCursorShape(tmk_CursorShape_Underline, false);
  tmk_readKeyEvent(-1, NULL, NULL);
  tmk_setCursorShape(tmk_CursorShape_Block, true);
  tmk_readKeyEvent(-1, NULL, NULL);
  tmk_setCursorShape(tmk_CursorShape_Bar, false);
  tmk_readKeyEvent(-1, NULL, NULL);
  tmk_resetCursorShape();
  return 0;
}
```

### tmk_resetCursorShape Function

#### Brief

Resets the terminal cursor shape.

#### Declaration

```c
void tmk_resetCursorShape(void);
```

#### Example

See the example from the [`tmk_setCursorShape`](#tmk_setcursorshape-function) function.

### tmk_setCursorVisibility Function

#### Brief

Sets the terminal cursor visibility.

#### Declaration

```c
void tmk_setCursorVisibility(bool isVisible);
```

#### Parameters

- `isVisible`: a boolean that states the cursor should be visible.

#### Example

```c
#include <tmk.h>

int main(void) {
  tmk_setCursorVisibility(false);
  tmk_readKeyEvent(-1, NULL, NULL);
  tmk_setCursorVisibility(true);
  return 0;
}
```

### tmk_readKeyEvent Function

#### Brief

Reads a terminal key event and remove it from the input buffer. Content typed and pasted into the terminal are considered key events. Under some situations, the terminal may not give enough information for the library to distinguish certain key events and some key sequences may be reserved by the terminal or operating system. Modifiers keys may only be identified in keys represented by UTF-8 graphemes. During execution, it blocks the receipt of signals. For convenience, it flushes the terminal output buffer upon execution. To avoid reading cached events, use it with the [`tmk_clearInputBuffer`](#tmk_clearinputbuffer-function) function.

#### Declaration

```c
int tmk_readKeyEvent(int16_t waitInMilliseconds, struct tmk_KeyEvent *event,
                     bool (*filter)(struct tmk_KeyEvent *));
```

#### Parameters

- `waitInMilliseconds`: the time in milliseconds to wait for an event. If the time is negative, it waits forever; If zero, it returns immediately; Or, if positive, it waits the given time.
- `event`: the address where the event information may be put into. If not necessary, it may be set to NULL.
- `filter`: a function to filter events while the timer is running, avoiding to destroy it unless necessary. It receives a pointer to the event being parsed as a parameter and should return a boolean that states the event should be considered. It is only used when the wait specified is positive. If not necessary, set it to `NULL`.

#### Return Value

0 if successful; -1 if user interaction was not possible due to stream redirection or wide character orientation; -2 if no events were available; -3 if the timer ran out; Or -4 if the terminal window was resized during the reading.

#### Example

```c
#include <tmk.h>

static bool filter(struct tmk_KeyEvent *event) {
  return (event->key == 'a' && !event->modifiers) ||
         (event->key == 'b' && event->modifiers & tmk_ModifierKey_AltOption &&
          !(event->modifiers & tmk_ModifierKey_Ctrl)) ||
         (event->key == 'C' && !event->modifiers) ||
         event->key == tmk_Key_DownArrow || event->key == *(int *)"🐉";
}

int main(void) {
  tmk_write("Waiting for key events");
  for (int16_t timer = 5; timer; --timer) {
    tmk_write(" %d...", timer);
    struct tmk_KeyEvent event;
    int status = tmk_readKeyEvent(1000, &event, filter);
    if (!status || status == -4) {
      tmk_writeLine("");
      tmk_write(
          status == -4 ? "Interrupted by window resize action."
                       : "Pressed/Typed %s.",
          event.key == 'a' && !event.modifiers ? "the A key"
          : event.key == 'b' && event.modifiers & tmk_ModifierKey_AltOption &&
                  !(event.modifiers & tmk_ModifierKey_Ctrl)
              ? "the Alt/Option + B sequence"
          : (event.key == 'C' && !event.modifiers) ? "the Shift + C sequence"
          : event.key == tmk_Key_DownArrow         ? "the Down Arrow key"
          : event.key == *(int *)"🐉"              ? "the Dragon emoji"
                                                   : "");
      break;
    }
  }
  tmk_writeLine("");
  return 0;
}
```

### tmk_writeArguments Function

#### Brief

Formats and writes a string to the terminal output stream. On Windows, it sets the console output code page to UTF-8.

#### Declaration

```c
void tmk_writeArguments(const char *format, va_list arguments);
```

#### Parameters

- `format`: the format to be used. It accepts the same format specifiers as the `printf` function family.
- `arguments`: the arguments to be formatted.

#### Example

```c
#include <tmk.h>

static void writeInfo(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_write("INFO: ");
  tmk_writeArguments(format, arguments);
  va_end(arguments);
}

int main(void) {
  writeInfo("Here Be Dragons! %s", "skippyr");
  return 0;
}
```

### tmk_writeArgumentsLine Function

#### Brief

Formats and writes a string to the terminal output stream with a newline grapheme appended to its end. On Windows, it sets the console output code page to UTF-8.

#### Declaration

```c
void tmk_writeArgumentsLine(const char *format, va_list arguments);
```

#### Parameters

- `format`: the format to be used. It accepts the same format specifiers as the `printf` function family.
- `arguments`: the arguments to be formatted.

```c
#include <tmk.h>

static void writeInfo(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_write("INFO: ");
  tmk_writeArgumentsLine(format, arguments);
  va_end(arguments);
}

int main(void) {
  writeInfo("Here Be Dragons! %s", "skippyr");
  return 0;
}
```

### tmk_writeErrorArguments Function

#### Brief

Formats and writes a string to the terminal error stream. On Windows, it sets the console output code page to UTF-8. In order to synchronize possible ANSI escape sequences cached, it flushes the terminal output buffer upon execution.

#### Declaration

```c
void tmk_writeErrorArguments(const char *format, va_list arguments);
```

#### Parameters

- `format`: the format to be used. It accepts the same format specifiers as the `printf` function family.
- `arguments`: the arguments to be formatted.

#### Example

```c
#include <tmk.h>

static void writeError(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_writeError("ERROR: ");
  tmk_writeErrorArguments(format, arguments);
  va_end(arguments);
}

int main(void) {
  writeError("Here Be Dragons! %s", "skippyr");
  return 0;
}
```

### tmk_writeErrorArgumentsLine Function

#### Brief

Formats and writes a string to the terminal error stream with a newline grapheme appended to its end. On Windows, it sets the console output code page to UTF-8. In order to synchronize possible ANSI escape sequences cached, it flushes the terminal output buffer upon execution.

#### Declaration

```c
void tmk_writeErrorArgumentsLine(const char *format, va_list arguments);
```

#### Parameters

- `format`: the format to be used. It accepts the same format specifiers as the `printf` function family.
- `arguments`: the arguments to be formatted.

#### Example

```c
#include <tmk.h>

static void writeError(const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  tmk_writeError("ERROR: ");
  tmk_writeErrorArgumentsLine(format, arguments);
  va_end(arguments);
}

int main(void) {
  writeError("Here Be Dragons! %s", "skippyr");
  return 0;
}
```

### tmk_write Function

#### Brief

Formats and writes a string to the terminal output stream. On Windows, it sets the console output code page to UTF-8.

#### Declaration

```c
void tmk_write(const char *format, ...);
```

#### Parameters

- `format`: the format to be used. It accepts the same format specifiers as the `printf` function family.
- `...`: the arguments to be formatted.

#### Example

```c
#include <tmk.h>

int main(void) {
  tmk_write("Here Be Dragons!");
  return 0;
}
```

### tmk_writeLine Function

#### Brief

Formats and writes a string to the terminal output stream with a newline grapheme appended to its end. On Windows, it sets the console output code page to UTF-8.

#### Declaration

```c
void tmk_writeLine(const char *format, ...);
```

#### Parameters

- `format`: the format to be used. It accepts the same format specifiers as the `printf` function family.
- `...`: the arguments to be formatted.

#### Example

```c
#include <tmk.h>

int main(void) {
  tmk_writeLine("Here Be Dragons!");
  return 0;
}
```

### tmk_writeError Function

#### Brief

Formats and writes a string to the terminal error stream. On Windows, it sets the console output code page to UTF-8. In order to synchronize possible ANSI escape sequences cached, it flushes the terminal output buffer upon execution.

#### Declaration

```c
void tmk_writeError(const char *format, ...);
```

#### Parameters

- `format`: the format to be used. It accepts the same format specifiers as the `printf` function family.
- `...`: the arguments to be formatted.

#### Example

```c
#include <tmk.h>

int main(void) {
  tmk_writeError("Here Be Dragons!");
  return 0;
}
```

### tmk_writeErrorLine Function

#### Brief

Formats and writes a string to the terminal error stream with a newline grapheme appended to its end. On Windows, it sets the console output code page to UTF-8. In order to synchronize possible ANSI escape sequences cached, it flushes the terminal output buffer upon execution.

#### Declaration

```c
void tmk_writeErrorLine(const char *format, ...);
```

#### Parameters

- `format`: the format to be used. It accepts the same format specifiers as the `printf` function family.
- `...`: the arguments to be formatted.

#### Example

```c
#include <tmk.h>

int main(void) {
  tmk_writeErrorLine("Here Be Dragons!");
  return 0;
}
```

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>
