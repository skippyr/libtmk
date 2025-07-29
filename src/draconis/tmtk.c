#include "tmtk.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#define REDIRECTION_CACHE(fd) (!_isatty(fd) << fd)
#else
#define REDIRECTION_CACHE(fd) (!isatty(fd) << fd)
#endif
#define HAS_INIT (1 << 7)
#define INPUT 0
#define OUTPUT 1
#define ERROR 2

static void init(void);

static char cache = 0;

static void init(void) {
  if (cache & HAS_INIT) {
    return;
  }
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
  cache = HAS_INIT | REDIRECTION_CACHE(INPUT) | REDIRECTION_CACHE(OUTPUT) | REDIRECTION_CACHE(ERROR);
}

int is_input_redirected(void) {
  init();
  return !!(cache & INPUT);
}

int is_output_redirected(void) {
  init();
  return !!(cache & OUTPUT);
}

int is_error_redirected(void) {
  init();
  return !!(cache & ERROR);
}
