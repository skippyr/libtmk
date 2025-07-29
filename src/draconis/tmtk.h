#ifndef _DRACONIS_TMTK_H
#define _DRACONIS_TMTK_H
#include <stdarg.h>

enum Stream {
  Stream_Input,
  Stream_Output,
  Stream_Error
};

enum ANSIColor {
  ANSIColor_DarkBlack,
  ANSIColor_DarkRed,
  ANSIColor_DarkGreen,
  ANSIColor_DarkYellow,
  ANSIColor_DarkBlue,
  ANSIColor_DarkMagenta,
  ANSIColor_DarkCyan,
  ANSIColor_DarkWhite,
  ANSIColor_LightBlack,
  ANSIColor_LightRed,
  ANSIColor_LightGreen,
  ANSIColor_LightYellow,
  ANSIColor_LightBlue,
  ANSIColor_LightMagenta,
  ANSIColor_LightCyan,
  ANSIColor_LightWhite
};

enum FontEffect {
  FontEffect_Bold = 1,
  FontEffect_Dim = 1 << 1,
  FontEffect_Italic = 1 << 2,
  FontEffect_Underline = 1 << 3,
  FontEffect_DoubleUnderline = 1 << 4,
  FontEffect_SquigglyLine = 1 << 5,
  FontEffect_Strikethrough = 1 << 6,
  FontEffect_SlowBlinking = 1 << 7,
  FontEffect_RapidBlinking = 1 << 8,
  FontEffect_SwappedLayers = 1 << 9,
  FontEffect_Hidden = 1 << 10,
  FontEffect_Fraktur = 1 << 11
};

enum Layer {
  Layer_Foreground = 3,
  Layer_Background,
  Layer_Underline
};

struct RGBColor {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

#ifdef __cplusplus
extern "C" {
#endif
  int isStreamRedirected(int stream);
  void flushOutput(void);
  void outputWriteArguments(const char *format, va_list arguments);
  void outputWrite(const char *format, ...);
  void errorWriteArguments(const char *format, va_list arguments);
  void errorWrite(const char *format, ...);
  void setFontANSIColor(int color, int layer);
  void setFontRGBColor(struct RGBColor, int layer);
  void setFontEffects(int effects);
  void resetFontColor(int layer);
  void resetFontEffects(int effects);
#ifdef __cplusplus
}
#endif
#endif
