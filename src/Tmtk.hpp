#pragma once
#include <iostream>

#ifdef _WIN32
class CannotSetOutputCPException final : std::exception
{
};

class InvalidHandleValueException final : std::exception
{
};

class NoAnsiSupportException final : std::exception
{
};
#endif

class Terminal final
{
private:
    static bool s_isInputRedirected;
    static bool s_isOutputRedirected;
    static bool s_isErrorRedirected;
#ifdef _WIN32
    static HANDLE s_inputHandle;
    static HANDLE s_outputHandle;
    static HANDLE s_errorHandle;
#endif
    static bool s_hasInit;
    static bool s_hasAnsiCache;
    static bool s_ansiPrefersStdOut;

    static void Init();
#ifdef _WIN32
    static HANDLE GetHandle(DWORD id);
    static bool EnableAnsiParse(HANDLE handle);
#endif

    Terminal() = delete;

public:
    static bool IsInputRedirected();
    static bool IsOutputRedirected();
    static bool IsErrorRedirected();
    static void FlushOutputBuffer();

    template <typename... Arguments>
    static void Write(const std::string_view& format, Arguments... arguments)
    {
        Init();
        std::string result = std::vformat(format, std::make_format_args(arguments...));
        std::cout << result;
        s_ansiPrefersStdOut = true;
        if (s_hasAnsiCache && result.contains('\n'))
        {
            s_hasAnsiCache = false;
        }
    }

    template <typename... Arguments>
    static void WriteLine(const std::string_view& format, Arguments... arguments)
    {
        Write(format, arguments...);
        Write("\n");
    }

    template <typename... Arguments>
    static void WriteError(const std::string_view& format, Arguments... arguments)
    {
        Init();
        if (s_hasAnsiCache)
        {
            FlushOutputBuffer();
        }
        std::cerr << std::vformat(format, std::make_format_args(arguments...));
        s_ansiPrefersStdOut = false;
    }

    template <typename... Arguments>
    static void WriteErrorLine(const std::string_view& format, Arguments... arguments)
    {
        WriteError(format, arguments...);
        WriteError("\n");
    }
};
