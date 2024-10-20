#pragma once

namespace Tmk
{
    enum class CursorShape
    {
        /// <summary>
        /// Fills the whole cursor region.
        /// </summary>
        Block = 2,
        /// <summary>
        /// Fills a region at the bottom of the cursor region.
        /// </summary>
        Underline = 4,
        /// <summary>
        /// Fills a region at a horizontal side of the cursor region.
        /// </summary>
        Bar = 6
    };
}
