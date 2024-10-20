#include <Tmk/FontEffect.h>

namespace Tmk
{
    int operator|(FontEffect effectI, FontEffect effectII)
    {
        return static_cast<int>(effectI) | static_cast<int>(effectII);
    }
}
