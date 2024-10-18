#include "Coordinate.h"

namespace Tmk
{
    Coordinate::Coordinate(uint16_t column, uint16_t row) : m_column(column), m_row(row)
    {
    }

    uint16_t Coordinate::GetColumn() const
    {
        return m_column;
    }

    void Coordinate::SetColumn(uint16_t column)
    {
        m_column = column;
    }

    uint16_t Coordinate::GetRow() const
    {
        return m_row;
    }

    void Coordinate::SetRow(uint16_t row)
    {
        m_row = row;
    }
}
