#include <Tmk/Dimensions.h>

namespace Tmk
{
    Dimensions::Dimensions(std::uint16_t totalColumns, std::uint16_t totalRows) : m_totalColumns(totalColumns), m_totalRows(totalRows)
    {
    }

    std::uint16_t Dimensions::GetTotalColumns() const
    {
        return m_totalColumns;
    }

    std::uint16_t Dimensions::GetTotalRows() const
    {
        return m_totalRows;
    }

    std::uint32_t Dimensions::GetArea() const
    {
        return m_totalColumns * m_totalRows;
    }
}
