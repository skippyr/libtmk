#pragma once

#include <cstdint>

namespace Tmk
{
    /// <summary>Represents terminal dimensions.</summary>
    class Dimensions final
    {
    private:
        /// <summary>The total columns in the dimensions.</summary>
        std::uint16_t m_totalColumns;
        /// <summary>The total rows in the dimensions.</summary>
        std::uint16_t m_totalRows;

    public:
        /// <summary>Creates a new instance of the Dimensions class with the given dimensions.</summary>
        /// <param name="width">The total columns in the dimensions.</param>
        /// <param name="height">The total rows in the dimensions.</param>
        Dimensions(std::uint16_t totalColumns, std::uint16_t totalRows);
        /// <summary>Gets the total columns of the dimensions.</summary>
        /// <return>The total columns.</return>
        std::uint16_t GetTotalColumns() const;
        /// <summary>Gets the total rows of the dimensions.</summary>
        /// <return>The total rows.</return>
        std::uint16_t GetTotalRows() const;
        /// <summary>Gets the area of the dimensions.</summary>
        /// <return>The area.</return>
        std::uint32_t GetArea() const;
    };
}
