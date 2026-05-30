#include "Algorithms.h"

#include <sstream>

std::string Alg::ZeroLeadingInt(const int val)
{
    if (val < 10)
        return "0" + std::to_string(val);

    return std::to_string(val);
}

std::string Alg::TwoDecFloat(float val)
{
    val *= 100.f;
    int rounded = (int)(val + 0.5f);
    int whole = rounded / 100;
    int dec = rounded % 100;

    return std::to_string(whole) + "." + ZeroLeadingInt(dec);
}

float Alg::ParseTime(std::string str)
{
    bool is12Hr = (str.back() == 'p' || str.back() == 'P');
    if (is12Hr || (str.back() == 'a' || str.back() == 'A'))
    {
        str.pop_back();
    }

    if (str.find(':') == std::string::npos)
    {
        float result = std::stof(str);
        if (is12Hr)
        {
            int hour = (int)result;
            if (hour == 12)
            {
                return result;
            }
            return result + 12.f;
        }
        return result;
    }

    std::stringstream stream(str);
    std::string cell;
    int hour, min;

    std::getline(stream, cell, ':');
    hour = std::stoi(cell);
    std::getline(stream, cell);
    min = std::stoi(cell);

    float result = hour + (min / 60.f);

    if (is12Hr)
    {
        if (hour == 12)
        {
            return result;
        }
        return result + 12.f;
    }
    return result;
}

std::string Alg::TimeString12Hr(const float time)
{
    int hour = (int)time;
    int min = (int)((time - hour) * 60.f);
    std::string amPm = "am";
    if (hour >= 12)
    {
        amPm = "pm";
    }
    if (hour > 12)
    {
        hour -= 12;
    }

    return std::to_string(hour) + ":" + Alg::ZeroLeadingInt(min) + amPm;
}

std::string Alg::TimeStringDuration(const float time)
{
    int hour = (int)time;
    int min = (int)((time - hour) * 60.f);

    return std::to_string(hour) + "h " + Alg::ZeroLeadingInt(min) + "min";
}

std::string Alg::MoneyString(float amount)
{
    amount *= 100.f;
    int rounded = (int)(amount + 0.5f);
    int dollar = rounded / 100;
    int cents = rounded % 100;

    return "$" + std::to_string(dollar) + "." + ZeroLeadingInt(cents);
}

std::string Alg::Prespacing(const std::string& str, size_t totalWidth)
{
    size_t numSpaces = totalWidth - str.length();
    return std::string(numSpaces, ' ') + str;
}

std::string Alg::Postspacing(const std::string& str, size_t totalWidth)
{
    size_t numSpaces = totalWidth - str.length();
    return str + std::string(numSpaces, ' ');
}
