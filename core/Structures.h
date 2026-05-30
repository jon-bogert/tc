#pragma once

#include <string>
#include <cstdint>
#include <vector>

struct Date
{
	uint16_t year;
	uint8_t month;
	uint8_t day;

	static Date FromString(const std::string& str);
	static Date Today();
	static std::string DateTimestamp();

	std::string ToString(const char delim = '-') const;
	bool IsUnassigned() const { return year == 0; }

	bool operator==(const Date& other);
	bool operator!=(const Date& other);
};

struct Entry
{
	Date date;
	float start;
	float end = FLT_MAX;
	std::string note;
};

struct Period
{
	Date start;
	Date end;
	std::vector<Entry> entries;
};

class Table
{
public:
	void SetDimensions(size_t columns, size_t rows);
	const std::string& Get(size_t col, size_t row) const;
	void Set(size_t col, size_t row, const std::string& val);

	std::string ToString();

private:
	void Resize();
	std::vector<size_t> GetColumnSizes();

	size_t m_columns = 0;
	size_t m_rows = 0;
	std::vector<std::vector<std::string>> m_data;
};