#include <iostream>
#include <format>

namespace Tmk
{
    class Terminal final
    {
    private:
        Terminal() = delete;

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
        };

        template <int N>
        class WritableStream : public Stream<N>
        {
        private:
            WritableStream() = delete;

        public:
            static std::ostream& GetCppOStream()
            {
                return N == 1 ? std::cout : std::cerr;
            }

            template <typename... Args>
            static void Write(std::string_view format, Args... arguments)
            {
                GetCppOStream() << std::vformat(format, std::make_format_args(arguments...));
            }

            template <typename... Args>
            static void WriteLine(std::string_view format, Args... arguments)
            {
                Write(format, arguments...);
                GetCppOStream() << std::endl;
            }
        };

    public:
        class Output : public WritableStream<1>
        {
        };

        class Error : public WritableStream<2>
        {
        };
    };
}
