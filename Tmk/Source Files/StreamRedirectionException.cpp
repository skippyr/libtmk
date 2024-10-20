#include <Tmk/StreamRedirectionException.h>

namespace Tmk
{
    StreamRedirectionException::StreamRedirectionException(const std::string& message) : Exception(message)
    {
    }
}
