#pragma once

#if defined(_WIN32)
/// <summary>A string that contains the name of the operating system in use.</summary>
#define TMK_OPERATING_SYSTEM_NAME "Windows"
/// <summary>A boolean that states the operating system in use is Windows.</summary>
#define TMK_IS_OPERATING_SYSTEM_WINDOWS 1
/// <summary>A boolean that states the operating system in use is Linux.</summary>
#define TMK_IS_OPERATING_SYSTEM_LINUX 0
/// <summary>A boolean that states the operating system in use is MacOS.</summary>
#define TMK_IS_OPERATING_SYSTEM_MACOS 0
/// <summary>A boolean that states the operating system is unknown.</summary>
#define TMK_IS_OPERATING_SYSTEM_UNKNOWN 0
#if defined(_M_ARM)
/// <summary>A string that contains the name of the CPU architecture in use.</summary>
#define TMK_CPU_ARCHITECTURE_NAME "ARM32"
/// <summary>A boolean that states the CPU architecture is ARM32.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM32 1
/// <summary>A boolean that states the CPU architecture is ARM64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM64 0
/// <summary>A boolean that states the CPU architecture is x86.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86 0
/// <summary>A boolean that states the CPU architecture is x86_64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86_64 0
/// <summary>A boolean that states the CPU architecture is unknown.</summary>
#define TMK_IS_CPU_ARCHITECTURE_UNKNOWN 0
#elif defined(_M_ARM64)
/// <summary>A string that contains the name of the CPU architecture in use.</summary>
#define TMK_CPU_ARCHITECTURE_NAME "ARM64"
/// <summary>A boolean that states the CPU architecture is ARM32.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM32 0
/// <summary>A boolean that states the CPU architecture is ARM64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM64 1
/// <summary>A boolean that states the CPU architecture is x86.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86 0
/// <summary>A boolean that states the CPU architecture is x86_64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86_64 0
/// <summary>A boolean that states the CPU architecture is unknown.</summary>
#define TMK_IS_CPU_ARCHITECTURE_UNKNOWN 0
#elif defined(_WIN64)
/// <summary>A string that contains the name of the CPU architecture in use.</summary>
#define TMK_CPU_ARCHITECTURE_NAME "x86_64"
/// <summary>A boolean that states the CPU architecture is ARM32.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM32 0
/// <summary>A boolean that states the CPU architecture is ARM64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM64 0
/// <summary>A boolean that states the CPU architecture is x86.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86 0
/// <summary>A boolean that states the CPU architecture is x86_64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86_64 1
/// <summary>A boolean that states the CPU architecture is unknown.</summary>
#define TMK_IS_CPU_ARCHITECTURE_UNKNOWN 0
#else
/// <summary>A string that contains the name of the CPU architecture in use.</summary>
#define TMK_CPU_ARCHITECTURE_NAME "x86"
/// <summary>A boolean that states the CPU architecture is ARM32.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM32 0
/// <summary>A boolean that states the CPU architecture is ARM64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM64 0
/// <summary>A boolean that states the CPU architecture is x86.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86 1
/// <summary>A boolean that states the CPU architecture is x86_64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86_64 0
/// <summary>A boolean that states the CPU architecture is unknown.</summary>
#define TMK_IS_CPU_ARCHITECTURE_UNKNOWN 0
#endif
#else
#if defined(__linux__)
/// <summary>A string that contains the name of the operating system in use.</summary>
#define TMK_OPERATING_SYSTEM_NAME "Linux"
/// <summary>A boolean that states the operating system in use is Windows.</summary>
#define TMK_IS_OPERATING_SYSTEM_WINDOWS 0
/// <summary>A boolean that states the operating system in use is Linux.</summary>
#define TMK_IS_OPERATING_SYSTEM_LINUX 1
/// <summary>A boolean that states the operating system in use is MacOS.</summary>
#define TMK_IS_OPERATING_SYSTEM_MACOS 0
/// <summary>A boolean that states the operating system is unknown.</summary>
#define TMK_IS_OPERATING_SYSTEM_UNKNOWN 0
#elif defined(__APPLE__)
/// <summary>A string that contains the name of the operating system in use.</summary>
#define TMK_OPERATING_SYSTEM_NAME "MacOS"
/// <summary>A boolean that states the operating system in use is Windows.</summary>
#define TMK_IS_OPERATING_SYSTEM_WINDOWS 0
/// <summary>A boolean that states the operating system in use is Linux.</summary>
#define TMK_IS_OPERATING_SYSTEM_LINUX 0
/// <summary>A boolean that states the operating system in use is MacOS.</summary>
#define TMK_IS_OPERATING_SYSTEM_MACOS 1
/// <summary>A boolean that states the operating system is unknown.</summary>
#define TMK_IS_OPERATING_SYSTEM_UNKNOWN 0
#else
/// <summary>A string that contains the name of the operating system in use.</summary>
#define TMK_OPERATING_SYSTEM_NAME "Unknown Operating System"
/// <summary>A boolean that states the operating system in use is Windows.</summary>
#define TMK_IS_OPERATING_SYSTEM_WINDOWS 0
/// <summary>A boolean that states the operating system in use is Linux.</summary>
#define TMK_IS_OPERATING_SYSTEM_LINUX 0
/// <summary>A boolean that states the operating system in use is MacOS.</summary>
#define TMK_IS_OPERATING_SYSTEM_MACOS 0
/// <summary>A boolean that states the operating system is unknown.</summary>
#define TMK_IS_OPERATING_SYSTEM_UNKNOWN 1
/// <summary>A string that contains the name of the CPU architecture in use.</summary>
#define TMK_CPU_ARCHITECTURE_NAME "Unknown CPU Architecture"
/// <summary>A boolean that states the CPU architecture is ARM32.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM32 0
/// <summary>A boolean that states the CPU architecture is ARM64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM64 0
/// <summary>A boolean that states the CPU architecture is x86.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86 0
/// <summary>A boolean that states the CPU architecture is x86_64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86_64 0
/// <summary>A boolean that states the CPU architecture is unknown.</summary>
#define TMK_IS_CPU_ARCHITECTURE_UNKNOWN 1
#endif
#if defined(__linux__) || defined(__APPLE__)
#if defined(__arm__)
/// <summary>A string that contains the name of the CPU architecture in use.</summary>
#define TMK_CPU_ARCHITECTURE_NAME "ARM32"
/// <summary>A boolean that states the CPU architecture is ARM32.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM32 1
/// <summary>A boolean that states the CPU architecture is ARM64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM64 0
/// <summary>A boolean that states the CPU architecture is x86.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86 0
/// <summary>A boolean that states the CPU architecture is x86_64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86_64 0
/// <summary>A boolean that states the CPU architecture is unknown.</summary>
#define TMK_IS_CPU_ARCHITECTURE_UNKNOWN 0
#elif defined(__aarch64__)
/// <summary>A string that contains the name of the CPU architecture in use.</summary>
#define TMK_CPU_ARCHITECTURE_NAME "ARM64"
/// <summary>A boolean that states the CPU architecture is ARM32.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM32 0
/// <summary>A boolean that states the CPU architecture is ARM64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM64 1
/// <summary>A boolean that states the CPU architecture is x86.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86 0
/// <summary>A boolean that states the CPU architecture is x86_64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86_64 0
/// <summary>A boolean that states the CPU architecture is unknown.</summary>
#define TMK_IS_CPU_ARCHITECTURE_UNKNOWN 0
#elif defined(__x86_64__)
/// <summary>A string that contains the name of the CPU architecture in use.</summary>
#define TMK_CPU_ARCHITECTURE_NAME "x86_64"
/// <summary>A boolean that states the CPU architecture is ARM32.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM32 0
/// <summary>A boolean that states the CPU architecture is ARM64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM64 0
/// <summary>A boolean that states the CPU architecture is x86.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86 0
/// <summary>A boolean that states the CPU architecture is x86_64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86_64 1
/// <summary>A boolean that states the CPU architecture is unknown.</summary>
#define TMK_IS_CPU_ARCHITECTURE_UNKNOWN 0
#elif defined(__i386__)
/// <summary>A string that contains the name of the CPU architecture in use.</summary>
#define TMK_CPU_ARCHITECTURE_NAME "x86"
/// <summary>A boolean that states the CPU architecture is ARM32.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM32 0
/// <summary>A boolean that states the CPU architecture is ARM64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_ARM64 0
/// <summary>A boolean that states the CPU architecture is x86.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86 1
/// <summary>A boolean that states the CPU architecture is x86_64.</summary>
#define TMK_IS_CPU_ARCHITECTURE_X86_64 0
/// <summary>A boolean that states the CPU architecture is unknown.</summary>
#define TMK_IS_CPU_ARCHITECTURE_UNKNOWN 0
#endif
#endif
#endif
#if Tmk_IS_OPERATING_SYSTEM_WINDOWS
/// <summary>The minimum exit code accepted by the operating system.</summary>
#define TMK_MINIMUM_EXIT_CODE -2147483647
/// <summary>The minimum exit code accepted by the operating system.</summary>
#define TMK_MAXIMUM_EXIT_CODE 2147483647
#else
/// <summary>The minimum exit code accepted by the operating system.</summary>
#define TMK_MINIMUM_EXIT_CODE 0
/// <summary>The minimum exit code accepted by the operating system.</summary>
#define TMK_MAXIMUM_EXIT_CODE 255
#endif
