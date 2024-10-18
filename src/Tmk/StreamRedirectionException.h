#pragma once

#include "Exception.h"

namespace Tmk
{
    /// <summary>Represents an exception thrown when a set of terminal streams are redirected.</summary>
    class StreamRedirectionException final : public Exception
    {
    };
}
