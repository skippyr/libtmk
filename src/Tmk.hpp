#pragma once

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

    static void Init();
#ifdef _WIN32
    static HANDLE GetHandle(DWORD id);
    static bool EnableAnsiParse(DWORD handleId);
#endif

    Terminal() = delete;

public:
    static bool IsInputRedirected();
    static bool IsOutputRedirected();
    static bool IsErrorRedirected();
};
