#include "Operations.h"

#include "Algorithms.h"
#include "Timecard.h"
#include "Structures.h"
#include "Config.h"

#include <iostream>
#include <fstream>

void Op::Add(Timecard& timecard, Config& config, int argc, char** argv)
{
	size_t count = timecard.GetPeriodCount();
	if (count == 0)
	{
		std::cout << "There are currently no periods to set as current, use `tc period add` to create a new one\n";
		return;
	}

	if (argc <= 3)
	{
		std::cout << "Insufficent parameters > tc add <start> <end> \"Note\"" << std::endl;
		return;
	}
	if (argc == 4)
	{
		std::cout << "Please provide a note for this entry." << std::endl;
		return;
	}
	if (argc >= 6)
	{
		std::cout << "Please make sure your note is wapped in parethesis (\")" << std::endl;
		return;
	}

	Entry& entry = timecard.GetPeriod(timecard.GetPeriodCount() - 1).entries.emplace_back();
	entry.date = Date::Today();
	try
	{
		entry.start = Alg::ParseTime(argv[2]);
		entry.end = Alg::ParseTime(argv[3]);
	}
	catch (std::exception)
	{
		std::cout << "Error -- Could not parse time parameters. See `--help` for formatting info." << std::endl;
		return;
	}
	entry.note = argv[4];

	timecard.Save();
	if (config.periodIndex != timecard.GetPeriodCount() - 1)
	{
		config.periodIndex = timecard.GetPeriodCount() - 1;
		config.Save();
		std::cout << "Setting current pay period to latest." << std::endl;
	}
	std::cout << "Successfully added new entry." << std::endl;
}

void Op::PeriodList(const Timecard& timecard)
{
	size_t count = timecard.GetPeriodCount();
	Table table;
	table.SetDimensions(3, count + 1);
	table.Set(1, 0, "START");
	table.Set(2, 0, "END");

	for (size_t i = 0; i < count; ++i)
	{
		const Period& period = timecard.GetPeriod(i);
		table.Set(0, i + 1, std::to_string(i));
		table.Set(1, i + 1, period.start.ToString());
		if (period.end.IsUnassigned())
		{
			table.Set(2, i + 1, "--");
		}
		else
		{
			table.Set(2, i + 1, period.end.ToString());
		}
	}

	std::cout << table.ToString() << std::endl;
}

void Op::PeriodSet(const std::string& to, const Timecard& timecard, Config& config)
{
	size_t count = timecard.GetPeriodCount();

	if (count == 0)
	{
		std::cout << "There are currently no periods to set as current, use `tc period add` to create a new one\n";
		return;
	}

	if (to == "latest")
	{
		size_t index = count - 1;
		config.periodIndex = index;
		std::cout << "Setting period index to: " << index << std::endl;
		config.Save();
		return;
	}

	int index = -1;
	try
	{
		index = std::stoi(to);
	}
	catch (std::exception)
	{
		std::cout << "'latest' or a valid index integer must be provided.\n";
		return;
	}

	if (index < 0 || index >= count)
	{
		std::cout << "Provided index was out of range, see `tc period list`\n";
		return;
	}

	config.periodIndex = index;
	std::cout << "Setting period index to: " << index << std::endl;
	config.Save();
}

void Op::PeriodShow(const Timecard& timecard, int index)
{
	if (index < 0 || index >= timecard.GetPeriodCount())
	{
		std::cout << "Current index was out of range, set to latest with `tc period set latest`\n";
		return;
	}

	const Period& period = timecard.GetPeriod(index);
	Table table;
	table.SetDimensions(4, period.entries.size() + 1);
	table.Set(0, 0, "DATE");
	table.Set(1, 0, "START");
	table.Set(2, 0, "END");
	table.Set(3, 0, "NOTE");

	for (size_t i = 0; i < period.entries.size(); ++i)
	{
		const Entry& e = period.entries[i];
		table.Set(0, i + 1, e.date.ToString());
		table.Set(1, i + 1, Alg::TimeString12Hr(e.start));
		table.Set(2, i + 1, Alg::TimeString12Hr(e.end));
		table.Set(3, i + 1, e.note);
	}

	std::cout << table.ToString() << std::endl;
}

void Op::PeriodStart(Timecard& timecard, Config& config)
{
	timecard.AddNewPeriod();
	config.periodIndex = timecard.GetPeriodCount() - 1;
	timecard.Save();
	config.Save();
	std::cout << "New pay period added." << std::endl;
}

void Op::PeriodCalc(const Timecard& timecard, const Config& config)
{
	if (config.periodIndex < 0 || config.periodIndex >= timecard.GetPeriodCount())
	{
		std::cout << "Current index was out of range, set to latest with `tc period set latest`\n";
		return;
	}

	const Period& period = timecard.GetPeriod(config.periodIndex);

	float totalHours = 0.f;

	std::vector<Date> dates;
	std::vector<float> hours;
	std::vector<float> pay;

	for (size_t i = 0; i < period.entries.size(); ++i)
	{
		const Entry& entry = period.entries[i];
		if (dates.empty() || dates.back() != entry.date)
		{
			dates.push_back(entry.date);
			hours.emplace_back(0.f);
			pay.emplace_back(0.f);
		}

		float span = entry.end - entry.start;
		hours.back() += span;
		pay.back() += span * config.payRate;

		totalHours += span;
	}

	float totalPay = totalHours * config.payRate;

	Table table;
	table.SetDimensions(6, dates.size() + 1);
	table.Set(0, 0, "DATE");
	table.Set(1, 0, "HOURS");
	table.Set(2, 0, "PAY");
	for (size_t i = 0; i < dates.size(); ++i)
	{
		table.Set(0, i + 1, dates[i].ToString());
		table.Set(1, i + 1, Alg::TwoDecFloat(hours[i]));
		table.Set(2, i + 1, Alg::MoneyString(pay[i]));
	}


	std::cout << "PAY PERIOD: " << period.start.ToString() << " => " << (period.end.IsUnassigned() ? "Ongoing" : period.end.ToString()) << std::endl << std::endl;
	std::cout << table.ToString() << std::endl << std::endl;

	std::cout << "Total Hours: " << Alg::TwoDecFloat(totalHours) << std::endl << "Total Pay: " << Alg::MoneyString(totalPay) << std::endl << std::endl;
}

void Op::ExportCSV(const Timecard& timecard, const Config& config)
{
	std::filesystem::path exportPath = config.timecardFile.parent_path() / (config.timecardFile.stem().string() + ".csv");

	std::ofstream fileOut(exportPath);
	if (!fileOut.is_open())
	{
		std::cout << "Error -- Could not create or open CSV export file: " << exportPath << std::endl;
		return;
	}

	for (size_t i = 0; i < timecard.GetPeriodCount(); ++i)
	{
		const Period& period = timecard.GetPeriod(i);
		fileOut << period.start.ToString('/') << ',' << period.end.ToString('/') << ',' << std::endl;

		for (size_t j = 0; j < period.entries.size(); ++j)
		{
			const Entry& entry = period.entries[j];
			fileOut << entry.date.ToString('/') << ',' << std::to_string(entry.start) << ',' << std::to_string(entry.end) << ',' << entry.note << std::endl;
		}
		if (i != timecard.GetPeriodCount() - 1)
		{
			fileOut << ",,," << std::endl;
		}
	}

	std::cout << "CSV Export Done: " << exportPath.generic_string() << std::endl;
}
