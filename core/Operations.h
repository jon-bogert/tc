#pragma once

#include <string>

class Timecard;
struct Config;

namespace Op
{
	void Add(Timecard& timecard, Config& config, int argc, char** argv);

	void PeriodList(const Timecard& timecard);
	void PeriodSet(const std::string& to, const Timecard& timecard, Config& config);
	void PeriodShow(const Timecard& timecard, int index);
	void PeriodStart(Timecard& timecard, Config& config);
	void PeriodCalc(const Timecard& timecard, const Config& config, int argc, char** argv);

	void ExportCSV(const Timecard& timecard, const Config& config);
}