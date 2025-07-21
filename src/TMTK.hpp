#pragma once
#include <iostream>

namespace TMTK
{
#ifdef _WIN32
    class CannotSetOutputCPException final : std::exception
    {
    };

    class InvalidHandleValueException final : std::exception
    {
    };

    class NoANSISupportException final : std::exception
    {
    };
#endif

    class StreamRedirectionException final : std::exception
    {
    };

    enum class Layer
    {
        Foreground = 3,
        Background
    };

    enum class ANSIColor
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

    enum class FontWeight
    {
        Bold = 1,
        Dim
    };

    class RGBColor final
    {
        std::uint8_t m_red;
        std::uint8_t m_green;
        std::uint8_t m_blue;

    public:
        constexpr RGBColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue) noexcept : m_red(red), m_green(green), m_blue(blue)
        {
        }

        constexpr std::uint8_t GetRed() const noexcept
        {
            return m_red;
        }

        constexpr std::uint8_t GetGreen() const noexcept
        {
            return m_green;
        }

        constexpr std::uint8_t GetBlue() const noexcept
        {
            return m_blue;
        }

        constexpr void SetRed(std::uint8_t red) noexcept
        {
            m_red = red;
        }

        constexpr void SetGreen(std::uint8_t green) noexcept
        {
            m_green = green;
        }

        constexpr void SetBlue(std::uint8_t blue) noexcept
        {
            m_blue = blue;
        }
    };

    class Terminal final
    {
        static bool s_isInputRedirected;
        static bool s_isOutputRedirected;
        static bool s_isErrorRedirected;
#ifdef _WIN32
        static HANDLE s_inputHandle;
        static HANDLE s_outputHandle;
        static HANDLE s_errorHandle;
#endif
        static bool s_hasInit;
        static bool s_hasANSICache;
        static bool s_ansiPrefersStdOut;

        static void Init();
#ifdef _WIN32
        static HANDLE GetHandle(DWORD id);
        static bool EnableANSIParse(HANDLE handle);
#endif

        template <typename... Arguments>
        static void WriteAnsi(const std::string_view& format, Arguments... arguments)
        {
            if (s_ansiPrefersStdOut)
            {
                if (!s_isOutputRedirected)
                {
                    Write(format, arguments...);
                }
                else if (!s_isErrorRedirected)
                {
                    WriteError(format, arguments...);
                }
                else
                {
                    throw StreamRedirectionException();
                }
            }
            else
            {
                if (!s_isErrorRedirected)
                {
                    WriteError(format, arguments...);
                }
                else if (!s_isOutputRedirected)
                {
                    Write(format, arguments...);
                }
                else
                {
                    throw StreamRedirectionException();
                }
            }
        }

        Terminal() = delete;

    public:
        static bool IsInputRedirected();
        static bool IsOutputRedirected();
        static bool IsErrorRedirected();
        static void FlushOutputBuffer();
        static void SetFontForeground(std::uint8_t ansiColor);
        static void SetFontForeground(ANSIColor color);
        static void SetFontForeground(RGBColor color);
        static void SetFontBackground(std::uint8_t ansiColor);
        static void SetFontBackground(ANSIColor color);
        static void SetFontBackground(RGBColor color);
        static void SetFontWeight(FontWeight weight);
        static void ResetFontColors();
        static void ResetFontWeight();
        static void OpenAlternateWindow();
        static void CloseAlternateWindow();
        static void RingBell();

        template <typename... Arguments>
        static void Write(const std::string_view& format, Arguments... arguments)
        {
            Init();
            std::string result = std::vformat(format, std::make_format_args(arguments...));
            std::cout << result;
            s_ansiPrefersStdOut = true;
            if (s_hasANSICache && result.contains('\n'))
            {
                s_hasANSICache = false;
            }
        }

        template <typename T>
        static void Write(const T& argument)
        {
            Write("{}", argument);
        }

        template <typename... Arguments>
        static void WriteLine(const std::string_view& format, Arguments... arguments)
        {
            Write(format, arguments...);
            Write("\n");
        }

        template <typename T>
        static void WriteLine(const T& argument)
        {
            WriteLine("{}", argument);
        }

        template <typename... Arguments>
        static void WriteError(const std::string_view& format, Arguments... arguments)
        {
            Init();
            if (s_hasANSICache)
            {
                FlushOutputBuffer();
            }
            std::cerr << std::vformat(format, std::make_format_args(arguments...));
            s_ansiPrefersStdOut = false;
        }

        template <typename T>
        static void WriteError(const T& argument)
        {
            WriteError("{}", argument);
        }

        template <typename... Arguments>
        static void WriteErrorLine(const std::string_view& format, Arguments... arguments)
        {
            WriteError(format, arguments...);
            WriteError("\n");
        }

        template <typename T>
        static void WriteErrorLine(const T& argument)
        {
            WriteErrorLine("{}", argument);
        }
    };
}
