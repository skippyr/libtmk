#pragma once

#include <exception>

namespace Tmk
{
    class Exception : private std::exception
    {
    };
}
