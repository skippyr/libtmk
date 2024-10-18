#pragma once

#include <cstdint>

namespace Tmk
{
    /// <summary>Represents an RGB color.</summary>
    class RgbColor final
    {
    private:
        /// <summary>The red component of the color.</summary>
        std::uint8_t m_red;
        /// <summary>The green component of the color.</summary>
        std::uint8_t m_green;
        /// <summary>The blue component of the color.</summary>
        std::uint8_t m_blue;

    public:
        /// <summary>Creates an instance of the RGB class with the given color components.</summary>
        /// <param name="name">The red component of the color.</param>
        /// <param name="green">The green component of the color.</param>
        /// <param name="blue">The blue component of the color.</param>
        RgbColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue);
        /// <summary>Gets the red component of the color.</summary>
        /// <return>The component.</return>
        std::uint8_t GetRed() const;
        /// <summary>Gets the green component of the color.</summary>
        /// <return>The component.</return>
        std::uint8_t GetGreen() const;
        /// <summary>Gets the blue component of the color.</summary>
        /// <return>The component.</return>
        std::uint8_t GetBlue() const;
    };
}
