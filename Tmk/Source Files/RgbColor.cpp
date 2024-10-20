#include <Tmk/RgbColor.h>

namespace Tmk
{
    RgbColor::RgbColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue) : m_red(red), m_green(green), m_blue(blue)
    {
    }

    uint8_t RgbColor::GetRed() const
    {
        return m_red;
    }

    uint8_t RgbColor::GetGreen() const
    {
        return m_green;
    }

    uint8_t RgbColor::GetBlue() const
    {
        return m_blue;
    }
}
