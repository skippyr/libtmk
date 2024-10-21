#pragma once

namespace Tmk
{
    /// <summary>
    /// Contains the font effects.
    /// </summary>
    enum class FontEffect
    {
        /// <summary>
        /// Makes the text curly. It may require the use of a font with italic style.
        /// </summary>
        Italic = 1 << 3,
        /// <summary>
        /// Draws a horizontal line under the text.
        /// </summary>
        Underline = 1 << 4,
        /// <summary>
        /// Makes the text blink periodically.
        /// </summary>
        Blinking = 1 << 5,
        /// <summary>
        /// Swaps the foreground and background colors.
        /// </summary>
        ReverseVideo = 1 << 7,
        /// <summary>
        /// Makes the text hard to see or invisible.
        /// </summary>
        Conceal = 1 << 8,
        /// <summary>
        /// Draws a horizontal line in the middle of the text.
        /// </summary>
        Strikethrough = 1 << 9
    };

    /// <summary>
    /// Concatenates two effects together, creating a bitmask.
    /// </summary>
    /// <param name="effectI">The first effect.</param>
    /// <param name="effectII">The second effect.</param>
    int operator|(FontEffect effectI, FontEffect effectII);
}
