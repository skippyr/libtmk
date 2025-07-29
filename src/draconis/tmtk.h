#ifndef _draconis_tmtk_h
#define _draconis_tmtk_h
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

enum Layer {
  Layer_Foreground = 3,
  Layer_Background,
  Layer_Ornament
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
  void resetFontColor(int layer);
#ifdef __cplusplus
}
#endif
#endif
