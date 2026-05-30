#include "Structures.h"

#include "Algorithms.h"

#include <sstream>

void Table::SetDimensions(size_t columns, size_t rows)
{
    m_columns = columns;
    m_rows = rows;
    Resize();
}

const std::string& Table::Get(size_t col, size_t row) const
{
    return m_data[row][col];
}

void Table::Set(size_t col, size_t row, const std::string& val)
{
    m_data[row][col] = val;
}

std::string Table::ToString()
{
    std::vector<size_t> colSpacing = GetColumnSizes();
    std::stringstream os;

    for (size_t r = 0; r < m_rows; ++r)
    {
        for (size_t c = 0; c < m_columns; ++c)
        {
            os << Alg::Postspacing(m_data[r][c], colSpacing[c]) << "   ";
        }
        if (r != m_rows - 1)
        {
            os << '\n';
        }
    }

    return os.str();
}

void Table::Resize()
{
    m_data.resize(m_rows);
    for (size_t r = 0; r < m_rows; ++r)
    {
        m_data[r].resize(m_columns);
    }
}

std::vector<size_t> Table::GetColumnSizes()
{
    std::vector<size_t> result(m_columns);

    for (size_t r = 0; r < m_rows; ++r)
    {
        for (size_t c = 0; c < m_columns; ++c)
        {
            size_t length = m_data[r][c].length();
            result[c] = (length > result[c]) ? length : result[c];
        }
    }

    return result;
}
