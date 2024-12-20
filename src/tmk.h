#if !defined(_tmk_H)
#define _tmk_H
#if defined(_WIN32)
#define tmk_OS "Windows"
#define tmk_IS_WINDOWS_OS 1
#define tmk_IS_LINUX_OS 0
#define tmk_IS_MAC_OS 0
#define tmk_IS_UNKNOWN_OS 0
#if defined(_M_ARM)
#define tmk_CPU_ARCH "ARM32"
#define tmk_IS_ARM32_CPU_ARCH 1
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#elif defined(_M_ARM64)
#define tmk_CPU_ARCH "ARM64"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 1
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#elif defined(_WIN64)
#define tmk_CPU_ARCH "x86_64"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 1
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#else
#define tmk_CPU_ARCH "x86"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 1
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#endif
#else
#if defined(__linux__)
#define tmk_OS "Linux"
#define tmk_IS_WINDOWS_OS 0
#define tmk_IS_LINUX_OS 1
#define tmk_IS_MAC_OS 0
#define tmk_IS_UNKNOWN_OS 0
#elif defined(__APPLE__)
#define tmk_OS "MacOS"
#define tmk_IS_WINDOWS_OS 0
#define tmk_IS_LINUX_OS 0
#define tmk_IS_MAC_OS 1
#define tmk_IS_UNKNOWN_OS 0
#else
#define tmk_OS "Unknown Operating System"
#define tmk_IS_WINDOWS_OS 0
#define tmk_IS_LINUX_OS 0
#define tmk_IS_MAC_OS 0
#define tmk_IS_UNKNOWN_OS 1
#define tmk_CPU_ARCH "Unknown CPU Architecture"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 1
#endif
#if defined(__linux__) || defined(__APPLE__)
#if defined(__arm__)
#define tmk_CPU_ARCH "ARM32"
#define tmk_IS_ARM32_CPU_ARCH 1
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#elif defined(__aarch64__)
#define tmk_CPU_ARCH "ARM64"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 1
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#elif defined(__x86_64__)
#define tmk_CPU_ARCH "x86_64"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 0
#define tmk_IS_X86_64_CPU_ARCH 1
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#elif defined(__i386__)
#define tmk_CPU_ARCH "x86"
#define tmk_IS_ARM32_CPU_ARCH 0
#define tmk_IS_ARM64_CPU_ARCH 0
#define tmk_IS_X86_CPU_ARCH 1
#define tmk_IS_X86_64_CPU_ARCH 0
#define tmk_IS_UNKNOWN_CPU_ARCH 0
#endif
#endif
#endif
#if tmk_IS_WINDOWS_OS
#define tmk_MIN_EXIT_CODE -2147483647
#define tmk_MAX_EXIT_CODE 2147483647
#else
#define tmk_MIN_EXIT_CODE 0
#define tmk_MAX_EXIT_CODE 255
#endif

#include <stdarg.h>

enum tmk_Stream {
  tmk_Stream_Input,
  tmk_Stream_Output,
  tmk_Stream_Error
};

#if defined(__cplusplus)
extern "C" {
#endif
int tmk_isStreamRedirected(int stream);
void tmk_flushOutputBuffer(void);
void tmk_clearInputBuffer(void);
#if tmk_IS_WINDOWS_OS
char *tmk_convertUtf16ToUtf8(const wchar_t *utf16String, size_t *length);
wchar_t *tmk_convertUtf8ToUtf16(const char *utf8String, size_t *length);
#endif
#if defined(__cplusplus)
}
#endif
#endif
