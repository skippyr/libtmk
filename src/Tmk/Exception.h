#pragma once

#include <exception>

namespace Tmk
{
    /// <summary>Represents a generic exception to be thrown by the library.</summary>
    class Exception : private std::exception
    {
    };
}
