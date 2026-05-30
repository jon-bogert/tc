#pragma once

#include <datetime.h>

class Date
{
public:
	Date() = default;
	Date(int year, int month, int day);
	Date(const jed_utils::datetime& date) : m_date(date) {};

	static Date FromString(const std::string& str);
	static Date Today();
	static std::string DateTimestamp();
	static uint8_t MonthDayCount(uint8_t month, uint16_t year);

	int Day() const;
	int Month() const;
	int Year() const;

	void SetDay(const int day);
	void SetMonth(const int month);
	void SetYear(const int year);

	std::string ToString(const char delim = '-') const;
	bool IsUnassigned() const;
	int DaysBetween(const Date& other) const;

	bool operator==(const Date& other);
	bool operator!=(const Date& other);

	bool Date::operator<(const Date& other) const;
	bool Date::operator<=(const Date& other) const { return !(other < *this); }
	bool Date::operator>(const Date& other) const { return other < *this; }
	bool Date::operator>=(const Date& other) const { return !(*this < other); }

	Date operator+(int days) const;
	Date operator-(int days) const;

	Date& operator+=(int days);
	Date& operator-=(int days);

private:
	jed_utils::datetime m_date;
};