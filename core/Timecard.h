#pragma once

#include "Structures.h"

#include <filesystem>
#include <vector>

class Timecard
{
public:
	void Load(const std::filesystem::path& path);
	void Save();

	size_t GetPeriodCount() const { return m_periods.size(); }
	Period& GetPeriod(size_t index) { return m_periods[index]; }
	const Period& GetPeriod(size_t index) const { return m_periods[index]; }

	void AddNewPeriod();

private:
	bool MakeCopy();

	std::vector<Period> m_periods;
	std::filesystem::path m_path;
};