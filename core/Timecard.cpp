#include "Timecard.h"

#include "Algorithms.h"

#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>

void Timecard::Load(const std::filesystem::path& path)
{
	m_periods.clear();
	m_path = path;

	std::ifstream file(path);
	if (!file.is_open())
	{
		return;
	}

	YAML::Node root;


	try
	{
		root = YAML::Load(file);
	}
	catch (std::exception)
	{
		std::cout << "Error -- YAML syntax error while parsing file: " << path << std::endl;
		return;
	}
	file.close();

	try
	{
		for (const YAML::Node& periodData : root["periods"])
		{
			Period& currPeriod = m_periods.emplace_back();
			currPeriod.start = Date::FromString(periodData["start"].as<std::string>());
			if (periodData["end"].IsDefined())
			{
				currPeriod.end = Date::FromString(periodData["end"].as<std::string>());
			}

			if (periodData["entries"].IsDefined())
			{
				for (const YAML::Node& entryData : periodData["entries"])
				{
					Entry& currEntry = currPeriod.entries.emplace_back();
					currEntry.date = Date::FromString(entryData["date"].as<std::string>());
					currEntry.start = Alg::ParseTime(entryData["start"].as<std::string>());
					currEntry.end = Alg::ParseTime(entryData["end"].as<std::string>());
					if (entryData["note"].IsDefined())
					{
						currEntry.note = entryData["note"].as<std::string>();
					}
				}
			}
		}
	}
	catch (std::exception)
	{
		std::cout << "Error -- Bad data parsing. Common causes are bad date, or time formatting. File: " << path << std::endl;
		return;
	}
}

void Timecard::Save()
{
	if (!MakeCopy())
	{
		std::cout << "Could not make backup. Aborting timecard save." << std::endl;
		return;
	}

	YAML::Node root;

	for (size_t i = 0; i < m_periods.size(); ++i)
	{
		const Period& currPeriod = m_periods[i];
		YAML::Node periodData;

		periodData["start"] = currPeriod.start.ToString();
		if (!currPeriod.end.IsUnassigned())
		{
			periodData["end"] = currPeriod.end.ToString();
		}

		for (size_t j = 0; j < currPeriod.entries.size(); ++j)
		{
			const Entry& currEntry = currPeriod.entries[j];
			YAML::Node entryData;

			entryData["date"] = currEntry.date.ToString();
			entryData["start"] = currEntry.start;
			entryData["end"] = currEntry.end;
			entryData["note"] = currEntry.note;
			
			periodData["entries"].push_back(entryData);
		}

		root["periods"].push_back(periodData);
	}

	if (!std::filesystem::exists(m_path.parent_path()))
		std::filesystem::create_directories(m_path.parent_path());

	std::ofstream fileOut(m_path);
	if (!fileOut.is_open())
	{
		std::cout << "Error -- Could not create or open timecard file to save: " << m_path << std::endl;
		return;
	}

	fileOut << root;
}

void Timecard::AddNewPeriod()
{
	Date today = Date::Today();
	if (!m_periods.empty())
	{
		m_periods.back().end = today;
	}
	Period& period = m_periods.emplace_back();
	period.start = today;
}

bool Timecard::MakeCopy()
{
	std::cout << "Making Backup..." << std::endl;
	if (!std::filesystem::exists(m_path))
		return true;

	std::filesystem::path destination = m_path.parent_path() / ".tc_backup";
	if (!std::filesystem::exists(destination))
		std::filesystem::create_directories(destination);

	destination /= Date::DateTimestamp() + ".yaml";

	return std::filesystem::copy_file(m_path, destination);
}
