#include "Dimensions.h"

namespace Tmk
{
    Dimensions::Dimensions(uint16_t totalColumns, uint16_t totalRows) : m_totalColumns(totalColumns), m_totalRows(totalRows)
    {
    }

    uint16_t Dimensions::GetTotalColumns() const
    {
        return m_totalColumns;
    }

    uint16_t Dimensions::GetTotalRows() const
    {
        return m_totalRows;
    }

    uint32_t Dimensions::GetArea() const
    {
        return m_totalColumns * m_totalRows;
    }
}
