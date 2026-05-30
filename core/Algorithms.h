#pragma once

#include <string>

namespace Alg
{
	std::string ZeroLeadingInt(const int val);
	std::string TwoDecFloat(float val);
	float ParseTime(std::string str);
	std::string TimeString12Hr(const float time);
	std::string TimeStringDuration(const float time);

	std::string MoneyString(float amount);

	std::string Prespacing(const std::string& str, size_t totalWidth);
	std::string Postspacing(const std::string& str, size_t totalWidth);
}