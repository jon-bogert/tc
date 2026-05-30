#pragma once

#include <filesystem>

struct Config
{
	std::filesystem::path timecardFile;
	float payRate = 0.f;
	int periodIndex = -1;

	static Config Load();
	static void OpenPath();

	void Save();
};