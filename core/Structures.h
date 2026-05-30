#pragma once

#include <string>
#include <cstdint>
#include <vector>

#include "Date.h"

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