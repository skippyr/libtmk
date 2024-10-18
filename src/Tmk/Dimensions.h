#pragma once

#include <cstdint>

namespace Tmk
{
    /// <summary>Represents terminal dimensions.</summary>
    class Dimensions final
    {
    private:
        /// <summary>The total number of columns in the dimensions.</summary>
        std::uint16_t m_totalColumns;
        /// <summary>The total number of rows in the dimensions.</summary>
        std::uint16_t m_totalRows;

    public:
        /// <summary>Creates a new instance of the Dimensions class with the given dimensions.</summary>
        /// <param name="width">The total number of columns in the dimensions.</param>
        /// <param name="height">The total number of rows in the dimensions.</param>
        Dimensions(std::uint16_t totalColumns, std::uint16_t totalRows);
        /// <summary>Gets the total number of columns of the dimensions.</summary>
        /// <return>The total number of columns of the dimensions.</return>
        std::uint16_t GetTotalColumns() const;
        /// <summary>Gets the total number of rows of the dimensions.</summary>
        /// <return>The total number of rows of the dimensions.</return>
        std::uint16_t GetTotalRows() const;
        /// <summary>Gets the area of the dimensions.</summary>
        /// <return>The area of the dimensions.</return>
        std::uint32_t GetArea() const;
    };
}
