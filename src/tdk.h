/* //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
 * BSD-3-Clause License
 * Copyright (c) 2023, Sherman Rofeman <skippyr.developer@gmail.com>
 *
 * See the LICENSE file that comes in its source code for more details.
 * //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\// */

#if !defined(_TDK_H)
#define _TDK_H

#include <fcntl.h>
#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>

enum tdk_Brightness {
  tdk_Brightness_Default,
  tdk_Brightness_Bold,
  tdk_Brightness_Dim
};

enum tdk_Color {
  tdk_Color_Black,
  tdk_Color_Red,
  tdk_Color_Green,
  tdk_Color_Yellow,
  tdk_Color_Blue,
  tdk_Color_Magenta,
  tdk_Color_Cyan,
  tdk_Color_White,
  tdk_Color_Default = 9
};

enum tdk_Effect {
  tdk_Effect_Italic = 1 << 3,
  tdk_Effect_Underline = 1 << 4,
  tdk_Effect_Blinking = 1 << 5,
  tdk_Effect_ReverseVideo = 1 << 7,
  tdk_Effect_Conceal = 1 << 8,
  tdk_Effect_Strikethrough = 1 << 9
};

enum tdk_Key {
  tdk_Key_ArrowUp = -22,
  tdk_Key_ArrowDown,
  tdk_Key_ArrowRight,
  tdk_Key_ArrowLeft,
  tdk_Key_F1,
  tdk_Key_F2,
  tdk_Key_F3,
  tdk_Key_F4,
  tdk_Key_F5,
  tdk_Key_F6,
  tdk_Key_F7,
  tdk_Key_F8,
  tdk_Key_F9,
  tdk_Key_F10,
  tdk_Key_F11,
  tdk_Key_F12,
  tdk_Key_Insert,
  tdk_Key_Delete,
  tdk_Key_PageUp,
  tdk_Key_PageDown,
  tdk_Key_End,
  tdk_Key_Home,
  tdk_Key_Tab = 9,
  tdk_Key_Enter,
  tdk_Key_Escape = 27,
  tdk_Key_Space = 32,
  tdk_Key_Backspace = 127
};

enum tdk_Layer {
  tdk_Layer_Foreground = 3,
  tdk_Layer_Background
};

enum tdk_Shape {
  tdk_Shape_Default,
  tdk_Shape_BlinkingBlock,
  tdk_Shape_Block,
  tdk_Shape_BlinkingUnderline,
  tdk_Shape_Underline,
  tdk_Shape_BlinkingBar,
  tdk_Shape_Bar
};

#if defined(__cplusplus)
extern "C" {
#endif
void tdk_clearCursorLine(void);
int tdk_getCursorPosition(int *column, int *line);
int tdk_getScreenSize(int *columns, int *lines);
int tdk_readKey(void);
void tdk_ringBell(void);
void tdk_setAlternateScreen(int isToOpen);
void tdk_setColor(int layer, int color);
void tdk_setColorBrightness(int brightness);
int tdk_setCursorPosition(int column, int line);
void tdk_setCursorShape(int shape);
void tdk_setCursorVisibility(int isToMakeVisible);
void tdk_setTextEffect(int effect, int isToEnable);
#if defined(__cplusplus)
}
#endif /* defined(__cplusplus)  */
#endif /* defined(_TDK_H) */
