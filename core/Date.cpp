#include "Date.h"

#include <string>
#include <sstream>
#include <chrono>

Date::Date(int year, int month, int day)
{
    m_date = jed_utils::datetime(year, month, day);
}

Date Date::FromString(const std::string& str)
{
    std::stringstream stream(str);
    std::string cell;
    std::getline(stream, cell, '-');
    int year = static_cast<uint16_t>(std::stoi(cell));
    std::getline(stream, cell, '-');
    int month = static_cast<uint8_t>(std::stoi(cell));
    std::getline(stream, cell);
    int day = static_cast<uint8_t>(std::stoi(cell));

    return Date(year, month, day);
}

Date Date::Today()
{
    std::time_t now = std::time(nullptr);
    std::tm* local_tm = std::localtime(&now);
    
    int year = local_tm->tm_year + 1900;
    int month = local_tm->tm_mon + 1;
    int day = local_tm->tm_mday;
    
    return Date(year, month, day);
}

std::string Date::DateTimestamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    
    std::tm now_tm = *std::localtime(&now_c);
    
    int sec = (now_tm.tm_hour * 3600) + (now_tm.tm_min * 60) + now_tm.tm_sec;
    return Date::Today().ToString() + "_" + std::to_string(sec);
}

uint8_t Date::MonthDayCount(uint8_t month, uint16_t year)
{
	switch (month)
	{
	case 2:
		return (year % 4 == 0) ? 29 : 28;
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	default:
		return 30;
	}
}

int Date::Day() const
{
    return m_date.get_day();
}

int Date::Month() const
{
    return m_date.get_month();
}

int Date::Year() const
{
    return m_date.get_year();
}

void Date::SetDay(const int day)
{
    m_date = jed_utils::datetime(m_date.get_year(), m_date.get_month(), day);
}

void Date::SetMonth(const int month)
{
    m_date = jed_utils::datetime(m_date.get_year(), month, m_date.get_day());
}

void Date::SetYear(const int year)
{
    m_date = jed_utils::datetime(year, m_date.get_month(), m_date.get_day());
}

std::string Date::ToString(const char delim) const
{
    std::stringstream fmt;
    fmt << "yyyy" << delim << "MM" << delim << "dd";
    return m_date.to_string(fmt.str());
}

bool Date::IsUnassigned() const
{
    return m_date.get_year() == 0;
}

int Date::DaysBetween(const Date& other) const
{
    jed_utils::timespan span = other.m_date - m_date;
    return span.get_days();
}

bool Date::operator==(const Date& other)
{
    return m_date == other.m_date;
}

bool Date::operator!=(const Date& other)
{
    return  m_date != other.m_date;
}

bool Date::operator<(const Date& other) const
{
    return m_date < other.m_date;
}

Date Date::operator+(int days) const
{
    jed_utils::datetime result = m_date;
    result.add_days(days);
    return Date(result);
}

Date Date::operator-(int days) const
{
    jed_utils::datetime result = m_date;
    result.add_days(-days);
    return Date(result);
}

Date& Date::operator+=(int days)
{
    m_date.add_days(days);
    return *this;
}

Date& Date::operator-=(int days)
{
    m_date.add_days(-days);
    return *this;
}
