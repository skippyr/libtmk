#include <iostream>

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

namespace Tmk
{
    enum class Layer
    {
        Foreground = 3,
        Background
    };

    enum class FontWeight
    {
        Bold = 1,
        Dim
    };

    enum class AnsiColor
    {
        DarkBlack,
        DarkRed,
        DarkGreen,
        DarkYellow,
        DarkBlue,
        DarkMagenta,
        DarkCyan,
        DarkWhite,
        LightBlack,
        LightRed,
        LightGreen,
        LightYellow,
        LightBlue,
        LightMagenta,
        LightCyan,
        LightWhite
    };

    class StreamRedirectionException final : std::exception
    {
    };

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
    class EncodingConverter final
    {
    private:
        EncodingConverter() = delete;

    public:
        static std::string ConvertUtf16ToUtf8(const std::wstring& utf16String);
        static std::wstring ConvertUtf8ToUtf16(const std::string& utf8String);
    };
#endif

    class CommandLineArguments final
    {
    private:
        std::vector<std::string> m_utf8Arguments;
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        std::vector<std::wstring> m_utf16Arguments;
#endif

    public:
        CommandLineArguments(int totalMainArguments, const char** mainArguments);
        int GetTotalArguments() const;
        const std::vector<std::string>& GetUtf8Arguments() const;
#if TMK_IS_OPERATING_SYSTEM_WINDOWS
        const std::vector<std::wstring>& GetUtf16Arguments() const;
#endif
    };

    class RgbColor final
    {
    private:
        std::uint8_t m_red;
        std::uint8_t m_green;
        std::uint8_t m_blue;

    public:
        RgbColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue);
        std::uint8_t GetRed() const;
        void SetRed(std::uint8_t red);
        std::uint8_t GetGreen() const;
        void SetGreen(std::uint8_t green);
        std::uint8_t GetBlue() const;
        void SetBlue(std::uint8_t blue);
    };

    class Dimensions final
    {
    private:
        std::uint16_t m_width;
        std::uint16_t m_height;
        std::uint32_t m_area;

    public:
        Dimensions(std::uint16_t width, std::uint16_t height);
        std::uint16_t GetWidth() const;
        std::uint16_t GetHeight() const;
        std::uint32_t GetArea() const;
    };

    class Terminal final
    {
    private:
        Terminal() = delete;

        class StreamRedirectionCache final
        {
        private:
            bool m_isInputRedirected;
            bool m_isOutputRedirected;
            bool m_isErrorRedirected;

        public:
            StreamRedirectionCache();
            StreamRedirectionCache(bool isInputRedirected, bool isOutputRedirected, bool isErrorRedirected);
            bool IsRedirected(int fileNo) const;
        };

        class Driver final
        {
        private:
            Driver() = delete;

            static bool m_hasEnabledFeatures;
            static StreamRedirectionCache m_redirectionCache;

#if TMK_IS_OPERATING_SYSTEM_WINDOWS
            static void SetUtf8Encoding();
            static void EnableVirtualTerminalProcessing();
#endif

        public:
            static void EnableFeatures();

            template <typename... Args>
            static void SendAnsiSequence(std::string_view sequence, Args... arguments)
            {
                if (!Output::IsRedirected())
                {
                    Output::Write(sequence, arguments...);
                }
                else if (!Error::IsRedirected())
                {
                    Error::Write(sequence, arguments...);
                }
            }

            static StreamRedirectionCache& GetStreamRedirectionCache();
        };

        template <int N>
        class Stream
        {
        private:
            Stream() = delete;

        public:
            static int GetFileNo()
            {
                return N;
            }

            static bool IsRedirected()
            {
                Driver::EnableFeatures();
                return Driver::GetStreamRedirectionCache().IsRedirected(GetFileNo());
            }
        };

        template <int N>
        class WritableStream : public Stream<N>
        {
        private:
            WritableStream() = delete;

            static std::ostream& GetCppOStream()
            {
                return N == 1 ? std::cout : std::cerr;
            }

        public:
            template <typename... Args>
            static void Write(std::string_view format, Args... arguments)
            {
                Driver::EnableFeatures();
                GetCppOStream() << std::vformat(format, std::make_format_args(arguments...));
            }

            template <typename... Args>
            static void WriteLine(std::string_view format, Args... arguments)
            {
                Write(format, arguments...);
                GetCppOStream() << std::endl;
            }

            static void WriteLine()
            {
                Driver::EnableFeatures();
                GetCppOStream() << std::endl;
            }
        };

    public:
        class Input final : public Stream<0>
        {
        };

        class Output final : public WritableStream<1>
        {
        };

        class Error final : public WritableStream<2>
        {
        };

        class Font final
        {
        private:
            Font() = delete;

        public:
            static void SetWeight(FontWeight weight);
            static void ResetWeight();
            static void SetColor(AnsiColor color, Layer layer);
            static void SetColor(RgbColor color, Layer layer);
            static void ResetColors();
        };

        class Window final
        {
        private:
            Window() = delete;

        public:
            static Dimensions GetDimensions();
        };
    };
}
