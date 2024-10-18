#pragma once

#include <cstdint>

namespace Tmk
{
    /// <summary>Represents a terminal coordinate.</summary>
    class Coordinate final
    {
    private:
        /// <summary>The column component.</summary>
        std::uint16_t m_column;
        /// <summary>The row component.</summary>
        std::uint16_t m_row;

    public:
        /// <summary>Creates an instance of the Coordinate class with the given components.</summary>
        /// <param name="column">The column component.</param>
        /// <param name="row">The row component.</param>
        Coordinate(std::uint16_t column, std::uint16_t row);
        /// <summary>Gets the column component.</summary>
        /// <return>The component.</return>
        std::uint16_t GetColumn() const;
        /// <summary>Sets the column component.</summary>
        /// <param name="column">The component to be set.</param>
        void SetColumn(std::uint16_t column);
        /// <summary>Gets the row component.</summary>
        /// <return>The component.</return>
        std::uint16_t GetRow() const;
        /// <summary>Sets the row component.</summary>
        /// <param name="row">The component to be set.</param>
        void SetRow(std::uint16_t row);
    };
}
