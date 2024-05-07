#ifndef _tdk_H
#define _tdk_H
enum tdk_Color {
  tdk_Color_Default = -1,
  tdk_Color_Black,
  tdk_Color_Red,
  tdk_Color_Green,
  tdk_Color_Yellow,
  tdk_Color_Blue,
  tdk_Color_Magenta,
  tdk_Color_Cyan,
  tdk_Color_White,
  tdk_Color_LightBlack,
  tdk_Color_LightRed,
  tdk_Color_LightGreen,
  tdk_Color_LightYellow,
  tdk_Color_LightBlue,
  tdk_Color_LightMagenta,
  tdk_Color_LightCyan,
  tdk_Color_LightWhite
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

enum tdk_Effect {
  tdk_Effect_Italic = 1 << 3,
  tdk_Effect_Underline = 1 << 4,
  tdk_Effect_Blinking = 1 << 5,
  tdk_Effect_ReverseVideo = 1 << 7,
  tdk_Effect_Conceal = 1 << 8,
  tdk_Effect_Strikethrough = 1 << 9
};

enum tdk_Key {
  tdk_Key_UpArrow = -23,
  tdk_Key_DownArrow,
  tdk_Key_RightArrow,
  tdk_Key_LeftArrow,
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
  tdk_Key_Home,
  tdk_Key_Insert,
  tdk_Key_Delete,
  tdk_Key_End,
  tdk_Key_PageUp,
  tdk_Key_PageDown,
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

enum tdk_Modifier {
  tdk_Modifier_Alt = 1,
  tdk_Modifier_Ctrl = 1 << 1
};

enum tdk_Weight {
  tdk_Weight_Default,
  tdk_Weight_Bold,
  tdk_Weight_Dim
};

#ifdef __cplusplus
extern "C" {
#endif
void tdk_clearCursorLine(void);
void tdk_clearInputBuffer(void);
int tdk_getCursorCoordinate(unsigned short *column, unsigned short *row);
int tdk_getWindowDimensions(unsigned short *totalColumns,
                            unsigned short *totalRows);
int tdk_readKeyEvent(int *key, int *modifiers);
void tdk_ringBell(void);
void tdk_set256Color(int color, int layer);
void tdk_setAlternateWindow(int isToEnable);
void tdk_setCursorCoordinate(unsigned short column, unsigned short row);
void tdk_setCursorShape(int shape);
void tdk_setCursorVisibility(int isToShow);
void tdk_setEffect(int effect, int isToEnable);
void tdk_setHexColor(int color, int layer);
void tdk_setRGBColor(int red, int green, int blue, int layer);
void tdk_setWeight(int weight);
#ifdef __cplusplus
}
#endif
#endif
