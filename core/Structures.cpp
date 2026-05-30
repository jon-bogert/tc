#include "Structures.h"

#include "Algorithms.h"

#include <sstream>
#include <ctime>
#include <chrono>

Date Date::FromString(const std::string& str)
{
    Date result;

    std::stringstream stream(str);
    std::string cell;
    std::getline(stream, cell, '-');
    result.year = static_cast<uint16_t>(std::stoi(cell));
    std::getline(stream, cell, '-');
    result.month = static_cast<uint8_t>(std::stoi(cell));
    std::getline(stream, cell);
    result.day = static_cast<uint8_t>(std::stoi(cell));

    return result;
}

Date Date::Today()
{
    Date result;

    std::time_t now = std::time(nullptr);
    std::tm* local_tm = std::localtime(&now);

    result.year = static_cast<uint16_t>(local_tm->tm_year + 1900);
    result.month = static_cast<uint8_t>(local_tm->tm_mon + 1);
    result.day = static_cast<uint8_t>(local_tm->tm_mday);

    return result;
}

std::string Date::DateTimestamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm now_tm = *std::localtime(&now_c);

    int sec = (now_tm.tm_hour * 3600) + (now_tm.tm_min * 60) + now_tm.tm_sec;
    return Date::Today().ToString() + "_" + std::to_string(sec);
}

std::string Date::ToString(const char delim) const
{
    if (IsUnassigned())
        return "";

    return std::to_string(year) + delim + Alg::ZeroLeadingInt(month) + delim + Alg::ZeroLeadingInt(day);
}

bool Date::operator==(const Date& other)
{
    return other.year == year && other.month == month && other.day == day;
}

bool Date::operator!=(const Date& other)
{
    return other.year != year || other.month != month || other.day != day;
}

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
