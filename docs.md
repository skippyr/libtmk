<p align="center">
    <img alt="" src="assets/ornament.webp" />
</p>
<h1 align="center">≥v≥v&ensp;libtdk&ensp;≥v≥v</h1>
<p align="center">Terminal Development Kit - Documentation</p>

## ❡ Reserved Names

This library reserves all names starting with `tdk` and `_tdk`. The underscore at the start indicates that that name is reserved for the library implementation.

## ❡ Enums

### tdk_Color Enum

#### Brief

Contains the first 16 colors of the xterm 256 colors palette plus one specifically for resets.

#### Enumerators

- `tdk_Color_Default`: the default terminal color. Used for resets.
- `tdk_Color_Black`, `tdk_Color_LightBlack`: the regular and light variants of the black color. They are the same as values `0` and `8`, respectively.
- `tdk_Color_Red`, `tdk_Color_LightRed`: the regular and light variants of the red color. They are the same as values `1` and `9`, respectively.
- `tdk_Color_Green`, `tdk_Color_LightGreen`: the regular and light variants of the green color. They are the same as values `2` and `10`, respectively.
- `tdk_Color_Yellow`, `tdk_Color_LightYellow`: the regular and light variants of the yellow color. They are the same as values `3` and `11`, respectively.
- `tdk_Color_Blue`, `tdk_Color_LightBlue`: the regular and light variants of the blue color. They are the same as values `4` and `12`, respectively.
- `tdk_Color_Magenta`, `tdk_Color_LightMagenta`: the regular and light variants of the magenta color. They are the same as values `5` and `13`, respectively.
- `tdk_Color_Cyan`, `tdk_Color_LightCyan`: the regular and light variants of the cyan color. They are the same as values `6` and `14`, respectively.
- `tdk_Color_White`, `tdk_Color_LightWhite`: the regular and light variants of the white color. They are the same as values `7` and `15`, respectively.

#### Comments

These colors are usually defined in terminal themes, thus they provide the best compatibility with the active theme.

### tdk_CursorShape Enum

#### Brief

Contains the terminal cursor shapes.

#### Fields

- `tdk_CursorShape_Default`: the default shape. Used for resets.
- `tdk_CursorShape_Underline`, `tdk_CursorShape_BlinkingUnderline`: the regular and blinking variants of the underline shape.
- `tdk_CursorShape_Block`, `tdk_CursorShape_BlinkingBlock`: the regular and blinking variants of the block shape.
- `tdk_CursorShape_Bar`, `tdk_CursorShape_BlinkingBar`: the regular and blinking variants of the bar shape.

### tdk_Effect Enum

#### Brief

Contains the terminal effects.

##### Fields

- `tdk_Effect_Italic`: it makes the text look curly. It may required the use of a font with italic style.
- `tdk_Effect_Underline`: it draws a horizontal line crossing below the text.
- `tdk_Effect_Blinking`: it makes the text blink periodically.
- `tdk_Effect_ReverseVideo`: it swaps the foreground and background colors.
- `tdk_Effect_Conceal`: it makes the text invisible or hard to see.
- `tdk_Effect_Strikethrough`: it draws a horizontal line crossing in the middle of the text.

### tdk_Key Enum

#### Brief

Contains keys not represented by UTF-8 graphemes that may be interpreted in a terminal key event.

#### Enumerators

- `tdk_Key_UpArrow`, `tdk_Key_RightArrow`, `tdk_Key_DownArrow`, `tdk_Key_LeftArrow`: the arrow keys.
- `tdk_Key_F1` to `tdk_Key_F12`: the function keys.
- `tdk_Key_Insert`, `tdk_Key_Delete`, `tdk_Key_Home`, `tdk_Key_End`, `tdk_Key_PageUp`, `tdk_Key_PageDown`: the navigation keys.
- `tdk_Key_Enter`, `tdk_Key_Space`, `tdk_Key_Backspace`, `tdk_Key_Tab`: the insertion/deletion keys.

### tdk_Layer Enum

#### Brief

Contains the terminal layers used as reference when setting colors.

#### Enumerators

- `tdk_Layer_Foreground`: the foreground layer.
- `tdk_Layer_Background`: the background layer.

### tdk_ModifierKey Enum

#### Brief

Contains the modifier key flags that may be set in a key event.

#### Enumerators

- `tdk_ModifierKey_Ctrl`: indicates if the `Ctrl` modifier key was being hold.
- `tdk_ModifierKey_Alt`: indicates if the `Alt` modifier key was being hold.

#### Comments

The modifier key `Shift` may be identified by checking the key value received during the event, as it usually affects it. For example, when `Shift` is used, the graphemes from `a` to `z` become uppercase.

### tdk_Stream Enum

#### Brief

Contains the standard terminal streams.

#### Enumerators

- `tdk_Stream_Input`: the standard input stream (`stdin`), from where a program receives graphemes/events.
- `tdk_Stream_Output`: the standard output stream (`stdout`), where a program outputs regular tense messages.
- `tdk_Stream_Error`: the standard error stream (`stderr`), where a program outputs error tense messages.

### tdk_Weight Enum

#### Brief

Contains the terminal weights for the foreground.

#### Enumerators

- `tdk_Weight_Default`: the default weight. Used for resets.
- `tdk_Weight_Bold`: the bold weight, usually rendered with bold weight and/or with bright colors.
- `tdk_Weight_Dim`: the dim weight, usually rendered with faint colors.

## ❡ Structs

### tdk_Coordinate Struct

#### Brief

Describes a coordinate in the terminal window.

#### Declaration

```c
struct tdk_Coordinate {
  unsigned short column;
  unsigned short row;
};
```

#### Fields

- `column`: the column component of the coordinate.
- `row`: the row component of the coordinate.

#### Comments

The coordinate system considers an origin coordinate, where `{.column = 0, .row = 0}`, at the top left corner of the window. From there, the column and row components increase going right and down, respectively.

### tdk_Dimensions Struct

#### Brief

Describes the dimensions of the terminal window.

#### Declaration

```c
struct tdk_Dimensions {
  unsigned short totalOfColumns;
  unsigned short totalOfRows;
};
```

#### Fields

- `totalOfColumns`: the total of columns in the terminal window.
- `totalOfRows`: the total of rows in the terminal window.

### tdk_KeyEvent Struct

#### Brief

Describes a terminal key event.

#### Declaration

```c
struct tdk_KeyEvent {
  int key;
  char modifiers;
};
```

#### Fields

- `key`: a value that identifies the key pressed. It may be an UTF-8 grapheme or a value from the [`tdk_Key`](#tdk_key-enum) enum.
- `modifiers`: a bitmask containing the modifier keys that were being hold during the event. Use bitwise AND (`&`) operations againts the flags defined in the [`tdk_ModifierKey`](#tdk_modifierkey-enum) enum to discover what has been set.

### tdk_RGB Struct

#### Brief

Describes an RGB color and its components.

#### Declaration

```c
struct tdk_RGB {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};
```

#### Fields

- `red`: the red component of the color. It must be a value from `0` to `255`.
- `green`: the green component of the color. It must be a value from `0` to `255`.
- `blue`: the blue component of the color. It must be a value from `0` to `255`.

## ❡ Functions

### tdk_clearCursorLine Function

#### Brief

Clears the visual contents present in the terminal cursor line.

#### Declaration

```c
void tdk_clearCursorLine(void);
```

#### Example

```c
#include <tdk.h>

int main(void) {
  tdk_write("Hello, world!");
  tdk_clearCursorLine();
  tdk_writeLine("Here Be Dragons!");
  return 0;
}
```

#### Comments

It does not do a thing if the standard output and error streams are being redirected or piped.

### tdk_clearInputBuffer Function

#### Brief

Clears the graphemes/events present in the terminal input buffer.

#### Declaration

```c
void tdk_clearInputBuffer(void);
```

#### Comments

- It does not do a thing if the standard input stream is being redirected or piped.
- On Linux, it fails if the standard input stream is oriented to wide characters.

#### Example

```c
#include <tdk.h>

int main(void) {
  tdk_clearInputBuffer();
  return 0;
}
```

### tdk_isTTY Function

#### Brief

Checks if a standard stream is connected to a terminal window (TTY).

#### Declaration

```c
int tdk_isTTY(int stream);
```

#### Parameters

- `stream`: the standard stream to be checked. It must be a value of the [`tdk_Stream`](#tdk_stream-enum) enum.

#### Return Value

If the stream is a TTY, it returns `1`, and `0` otherwise.

#### Example

```c
#include <tdk.h>

int main(void) {
  tdk_writeLine(":: Input : %d.", tdk_isTTY(tdk_Stream_Input));
  tdk_writeLine(":: Output: %d.", tdk_isTTY(tdk_Stream_Output));
  tdk_writeLine(":: Error : %d.", tdk_isTTY(tdk_Stream_Error));
  return 0;
}
```

### tdk_getCursorCoordinate Function

#### Brief

Gets the terminal cursor coordinate.

#### Declaration

```c
int tdk_getCursorCoordinate(struct tdk_Coordinate *coordinate);
```

#### Parameters

- `coordinate`: a pointer to a [`tdk_Coordinate`](#tdk_coordinate-struct) struct that receives the cursor coordinate.

#### Return Value

On success, it returns `0`, and `-1` otherwise. It fails:

- On Windows, if the standard output and error streams are being redirected or piped.
- On Linux, if the standard input stream or the standard output and error streams are being redirected or piped and if the input buffer is not empty. If you need to clear it, use the [`tdk_clearInputBuffer`](#tdk_clearinputbuffer-function) function.

#### Comments

The coordinate system considers an origin coordinate, where `{.column = 0, .row = 0}`, at the top left corner of the window. From there, the column and row components increase going right and down, respectively.

#### Example

```c
#include <tdk.h>

int main(void) {
  struct tdk_Coordinate cursorCoordinate;
  tdk_getCursorCoordinate(&cursorCoordinate);
  tdk_writeLine(":: Cursor Column: %hu.", cursorCoordinate.column);
  tdk_writeLine(":: Cursor Row   : %hu.", cursorCoordinate.row);
  return 0;
}
```

### tdk_getWindowDimensions Function

#### Brief

Gets the terminal window dimensions.

#### Declaration

```c
int tdk_getWindowDimensions(struct tdk_Dimensions *dimensions);
```

#### Parameters

- `dimensions`: a pointer to a [`tdk_Dimensions`](#tdk_dimensions-struct) struct that receives the window dimensions.

#### Return Value

On success, it returns `0`, and `-1` otherwise. It fails:

- On Windows, if the standard output and error streams are being redirected or piped.
- On Linux, if all the standard streams are being redirected or piped.

#### Example

```c
#include <tdk.h>

static struct tdk_Dimensions g_windowDimensions;

int main(void) {
  tdk_getWindowDimensions(&g_windowDimensions);
  tdk_writeLine(":: Total of columns: %hu.", g_windowDimensions.totalOfColumns);
  tdk_writeLine(":: Total of rows   : %hu.", g_windowDimensions.totalOfRows);
  return 0;
}
```

### tdk_readKeyEvent Function

#### Brief

Reads a key event from the input buffer and removes it from the buffer.

#### Declaration

```c
int tdk_readKeyEvent(struct tdk_KeyEvent *event);
```

#### Parameters

- `event`: A pointer to a [`tdk_KeyEvent`](#tdk_readkeyevent-struct) struct that receives information about the event.

#### Return Value

On success, it returns `0`, and `-1` otherwise. It fails:

- If the standard input or the standard output and error streams are being redirected or piped.
- On Linux, if the standard input stream is oriented to wide characters.

#### Comments

- It does not return until at least one valid key event is received.
- As a design choice of this library, modifier keys can only be identified in keys identified by UTF-8 graphemes.
- Some combinations of keys and modifier keys may be reserved to the terminal or operating system, thus making them not possible to be interpreted.
- Any content pasted on the terminal is interpreted as a key event. In theses cases, this function might leave non-read contents in the input buffer for future readings. If you need to clear it, use the [`tdk_clearInputBuffer`](#tdk_clearinputbuffer-function) function.

#### Examples

```c
#include <tdk.h>

int main(void) {
  struct tdk_KeyEvent keyEvent;
  tdk_readKeyEvent(&keyEvent);
  if (keyEvent.key == 'a' && keyEvent.modifiers & tdk_ModifierKey_Ctrl &&
     !(keyEvent.modifiers & tdk_ModifierKey_Alt)) {
    tdk_writeLine("Ctrl + A");
  } else if (keyEvent.key == 'B' && !keyEvent.modifiers) {
    tdk_writeLine("Shift + B");
  }
  else if (keyEvent.key == tdk_Key_UpArrow) {
    tdk_writeLine("Up Arrow");
  }
  else if (keyEvent.key >= tdk_Key_F1 && keyEvent.key <= tdk_Key_F12) {
    tdk_writeLine("Function Keys");
  }
  return 0;
}
```

### tdk_ringBell Function

#### Brief

Rings the terminal bell, possibly emitting a bell indicator in the title bar, window flash, system notification sound or a beep from the motherboard speaker.

#### Declaration

```c
void tdk_ringBell(void);
```

#### Comments

As it can be considered annoying, some terminal have their bell feature disabled by default.

#### Example

```c
#include <tdk.h>

int main(void) {
  tdk_ringBell();
  return 0;
}
```

### tdk_set256Color Function

#### Declaration

```c
void tdk_set256Color(int color, int layer);
```

#### Brief

Sets a color from the xterm 256 colors palette in a terminal layer or resets a color.

#### Parameters

- `color`: the color to be applied. It must be a value from `0` to `255` or one from the [`tdk_Color`](#tdk_color-enum) enum.
- `layer`: the layer to be affected. It must be a value from the [`tdk_Layer`](#tdk_layer-enum) enum.

#### Comments

- No color is applied if the standard output and error streams are being redirected or piped or if they are oriented to wide characters.
- If a custom color was applied, it must be manually reset before the end of the program to ensure the shell session does not get affected.

#### Example

```c
#include <tdk.h>

int main(void) {
  char message[] = "Here Be Dragons!";
  tdk_set256Color(tdk_Color_Red, tdk_Layer_Foreground);
  tdk_writeLine(message);
  tdk_set256Color(tdk_Color_Yellow, tdk_Layer_Foreground);
  tdk_writeLine(message);
  tdk_set256Color(tdk_Color_Green, tdk_Layer_Foreground);
  tdk_writeLine(message);
  tdk_set256Color(tdk_Color_Default, tdk_Layer_Foreground);
  tdk_writeLine(message);
  return 0;
}
```

### tdk_setAlternateWindow Function

#### Brief

Sets the state of the alternate window.

#### Declaration

```c
void tdk_setAlternateWindow(int isToEnable);
```

#### Parameters

- `isToEnable`: a boolean that indicates if the alternate window should be enabled, if non-zero, or disabled if zero.

#### Comments

- If the terminal does not have the alternate window feature, all changes will get applied to the primary window instead.
- The state of the alternate window can not be set if the standard output and error streams are being redirected or piped or if they are oriented to wide characters.
- If the alternate screen was enabled, it must be manually disabled before the end of the program to ensure the shell session does not get affected.

#### Example

```c
#include <tdk.h>

int main(void) {
  struct tdk_KeyEvent keyEvent;
  tdk_setAlternateWindow(1);
  tdk_write("Here Be Dragons!");
  tdk_readKeyEvent(&keyEvent);
  tdk_setAlternateWindow(0);
  return 0;
}
```

### tdk_setCursorCoordinate Function

#### Brief

Sets the terminal cursor coordinate.

#### Declaration

```c
int tdk_setCursorCoordinate(struct tdk_Coordinate coordinate);
```

#### Parameters

- `coordinate`: a [`tdk_Coordinate`](#tdk_coordinate-struct) struct containing the coordinate to be set.

#### Return Value

On success, it returns `0`, and `-1` otherwise. It fails:

- If the standard output and error streams are being redirected or piped or if they are oriented to wide characters.
- If the coordinate given is outside of the window boundaries.

#### Comments

The coordinate system considers an origin coordinate, where `{.column = 0, .row = 0}`, at the top left corner of the window. From there, the column and row components increase going right and down, respectively.

#### Example

```c
#include <tdk.h>

int main(void) {
  struct tdk_KeyEvent keyEvent;
  tdk_setAlternateWindow(1);
  tdk_setCursorCoordinate(4, 2);
  tdk_write("Here Be Dragons!");
  tdk_readKeyEvent(&keyEvent);
  tdk_setAlternateWindow(0);
  return 0;
}
```

### tdk_setCursorShape Function

#### Brief

Sets the terminal cursor shape.

#### Declaration

```c
void tdk_setCursorShape(int shape);
```

#### Parameters

- `shape`: the shape to be applied. It must be a value of the [`tdk_Shape`](#tdk_shape-enum) enum.

#### Comments

- No shape gets applied if the standard output and error streams are being redirected or piped or if they are oriented to wide characters.
- If a custom shape was applied, it must be manually reset before the end of the program to ensure the shell session does not get affected.

#### Example

```c
#include <tdk.h>

int main(void) {
  struct tdk_KeyEvent keyEvent;
  tdk_setCursorShape(tdk_CursorShape_Underline);
  tdk_readKeyEvent(&keyEvent);
  tdk_setCursorShape(tdk_CursorShape_Default);
  return 0;
}
```

### tdk_setCursorVisibility Function

#### Brief

Sets the visibility of the terminal cursor.

#### Declaration

```c
void tdk_setCursorVisibility(int isToShow);
```

#### Parameters

- `isToShow`: a boolean that indicates if the cursor should be shown, if non-zero, or hidden if zero.

#### Comments

- The cursor visibility can not be set if the standard output and error streams are being redirected or piped or if they are oriented to wide characters.
- If the cursor was hidden, its visibility must be manually reset before the end of the program to ensure the shell session does not get affected.

#### Example

```c
#include <tdk.h>

int main(void) {
  struct tdk_KeyEvent keyEvent;
  tdk_setCursorVisibility(0);
  tdk_readKeyEvent(&keyEvent);
  tdk_setCursorVisibility(1);
  return 0;
}
```

### tdk_setEffect Function

#### Brief

Sets the state of terminal effects.

#### Declaration

```c
void tdk_setEffect(int effect, int isToEnable);
```

#### Parameters

- `effect`: a bitmask containing the effects being targeted. It must be composed by the flags from the [`tdk_Effect`](#tdk_effect-enum) enum.
- `isToEnable`: a boolean that indicates if the effects should be enabled, if non-zero, or disabled if zero.

#### Comments

- Effects can not be set if the standard output and error streams are being redirected or piped or if they are oriented to wide characters.
- If a custom effect was applied, it must be manually reset before the end of the program to ensure the shell session does not get affected.

#### Example

```c
#include <tdk.h>

int main(void) {
  char message[] = "Here Be Dragons!";
  tdk_setEffect(tdk_Effect_Underline | tdk_Effect_ReverseVideo, 1);
  tdk_writeLine(message);
  tdk_setEffect(tdk_Effect_ReverseVideo, 0);
  tdk_writeLine(message);
  tdk_setEffect(tdk_Effect_Underline, 0);
  tdk_writeLine(message);
  return 0;
}
```

### tdk_setHEXColor Function

#### Brief

Sets a hex color in a terminal layer.

#### Declaration

```c
void tdk_setHEXColor(int color, int layer);
```

#### Parameters

- `color`: the color to be applied. It must be a value from `0` to `0xfffff`.
- `layer`: the layer to be affected. It must be a value from the [`tdk_Layer`](#tdk_layer-enum) enum.

#### Comments

- For this color to be shown, the terminal must have truecolor support.
- No color is applied if the standard output and error streams are being redirected or piped or if they are oriented to wide characters.
- If a custom color was applied, it must be manually reset before the end of the program to ensure the shell session does not get affected. For that purpose, use the [`tdk_set256Color`](#tdk_set256color-function) function.

#### Example

```c
#include <tdk.h>

int main(void) {
  char message[] = "Here Be Dragons!";
  tdk_setHEXColor(0xff0000, tdk_Layer_Foreground);
  tdk_writeLine(message);
  tdk_setHEXColor(0xffff00, tdk_Layer_Foreground);
  tdk_writeLine(message);
  tdk_setHEXColor(0x00ff00, tdk_Layer_Foreground);
  tdk_writeLine(message);
  tdk_set256Color(tdk_Color_Default, tdk_Layer_Foreground);
  tdk_writeLine(message);
  return 0;
}
```

### tdk_setRGBColor Function

#### Brief

Sets an RGB color in a terminal layer.

#### Declaration

```c
void tdk_setRGBColor(struct tdk_RGB color, int layer);
```

#### Parameters

- `color`: a [`tdk_RGB`](#tdk_rgb-struct) struct containing the components of the color to be applied.
- `layer`: the layer to be affected. It must be a value from the [`tdk_Layer`](#tdk_layer-enum) enum.

#### Comments

- For this color to be shown, the terminal must have truecolor support.
- No color is applied if the standard output and error streams are being redirected or piped or if they are oriented to wide characters.
- If a custom color was applied, it must be manually reset before the end of the program to ensure the shell session does not get affected. For that purpose, use the [`tdk_set256Color`](#tdk_set256color-function) function.

#### Example

```c
#include <tdk.h>

int main(void) {
  char message[] = "Here Be Dragons!";
  struct tdk_RGB red = {255, 0, 0};
  struct tdk_RGB yellow = {255, 255, 0};
  struct tdk_RGB green = {0, 255, 0};
  tdk_setRGBColor(red, tdk_Layer_Foreground);
  tdk_writeLine(message);
  tdk_setRGBColor(yellow, tdk_Layer_Foreground);
  tdk_writeLine(message);
  tdk_setRGBColor(green, tdk_Layer_Foreground);
  tdk_writeLine(message);
  tdk_set256Color(tdk_Color_Default, tdk_Layer_Foreground);
  tdk_writeLine(message);
  return 0;
}
```

### tdk_setWeight Function

#### Brief

Sets the weight of the terminal foreground.

#### Declaration

```c
void tdk_setWeight(int weight);
```

#### Parameters

- `weight`: the weight to be applied. It must be a value of the [`tdk_Weight`](#tdk_weight-enum) enum.

#### Comments

- No weight is applied if the standard output and error streams are being redirected or piped or if they are oriented to wide characters.
- If a custom weight was applied, it must be manually reset before the end of the program to ensure the shell session does not get affected.

#### Example

```c
#include <tdk.h>

int main(void) {
  char message[] = "Here Be Dragons!";
  tdk_setWeight(tdk_Weight_Bold);
  tdk_writeLine(message);
  tdk_setWeight(tdk_Weight_Dim);
  tdk_writeLine(message);
  tdk_setWeight(tdk_Weight_Default);
  tdk_writeLine(message);
  return 0;
}
```

### tdk_write Function

#### Brief

Writes an UTF-8 encoded string to the standard output stream based on a format, similar to `printf`, even accepting the same format specifiers.

#### Declaration

```c
int tdk_write(const char *format, ...);
```

#### Parameters

- `format`: the format to be used.
- `...`: a serie of arguments to be formatted.

#### Return Value

On success, it returns the number of bytes written excluding the `NULL` character, and `-1` otherwise. It fails if the standard output stream is oriented to wide characters.

#### Comments

On Windows, it sets the console page `CP_UTF8`.

#### Example

```c
#include <tdk.h>

int main(void) {
  tdk_write("Here Be Dragons!\n");
  return 0;
}
```

### tdk_writeError Function

#### Brief

Writes an UTF-8 encoded string to the standard error stream based on a format, similar to `printf`, even accepting the same format specifiers.

#### Declaration

```c
int tdk_writeError(const char *format, ...);
```

#### Parameters

- `format`: the format to be used.
- `...`: a serie of arguments to be formatted.

#### Return Value

On success, it returns the number of bytes written excluding the `NULL` character, and `-1` otherwise. It fails if the standard error stream is oriented to wide characters.

#### Comments

- For possible cached virtual escape sequences to be interpreted, it flushes the standard output buffer.
- On Windows, it sets the console page `CP_UTF8`.

#### Example

```c
#include <tdk.h>

int main(void) {
  tdk_writeError("Here Be Dragons!\n");
  return 0;
}
```

### tdk_writeErrorLine Function

#### Brief

Writes an UTF-8 encoded string to the standard error stream with a newline appended at its end and based on a format, similar to `printf`, even accepting the same format specifiers.

#### Declaration

```c
int tdk_writeErrorLine(const char *format, ...);
```

#### Parameters

- `format`: the format to be used.
- `...`: a serie of arguments to be formatted.

#### Return Value

On success, it returns the number of bytes written excluding the `NULL` character, and `-1` otherwise. It fails if the standard error stream is oriented to wide characters.

#### Comments

- For possible cached virtual escape sequences to be interpreted, it flushes the standard output buffer.
- On Windows, it sets the console page `CP_UTF8`.

#### Example

```c
#include <tdk.h>

int main(void) {
  tdk_writeErrorLine("Here Be Dragons!");
  return 0;
}
```

### tdk_writeLine Function

#### Brief

Writes an UTF-8 encoded string to the standard output stream with a newline appended at its end and based on a format, similar to `printf`, even accepting the same format specifiers.

#### Declaration

```c
int tdk_writeLine(const char *format, ...);
```

#### Parameters

- `format`: the format to be used.
- `...`: a serie of arguments to be formatted.

#### Return Value

On success, it returns the number of bytes written excluding the `NULL` character, and `-1` otherwise. It fails if the standard output stream is oriented to wide characters.

#### Comments

On Windows, it sets the console page `CP_UTF8`.

#### Example

```c
#include <tdk.h>

int main(void) {
  tdk_writeLine("Here Be Dragons!");
  return 0;
}
```

&ensp;
<p align="center"><sup><strong>≥v≥v&ensp;Here Be Dragons!&ensp;≥v≥</strong><br />Made with love by skippyr <3</sup></p>
