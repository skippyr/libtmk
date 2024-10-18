#include "RgbColor.h"

namespace Tmk
{
    RgbColor::RgbColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue) : m_red(red), m_green(green), m_blue(blue)
    {
    }

    RgbColor::RgbColor(std::uint32_t hex) {
    }

    std::uint8_t RgbColor::GetRed() const
    {
        return m_red;
    }

    std::uint8_t RgbColor::GetGreen() const
    {
        return m_green;
    }

    std::uint8_t RgbColor::GetBlue() const
    {
        return m_blue;
    }
}
