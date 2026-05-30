#include <iostream>

#include "Help.inl"

#include "Config.h"
#include "Timecard.h"
#include "Operations.h"

#define VER_NUM "1.1"

int main (int argc, char* argv[])
{
    if (argc <= 1)
    {
        Help();
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "help") == 0)
    {
        Help();
        return 0;
    }

    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "version") == 0)
    {
        std::cout << "Time Card by Jon \"Xepherin\" Bogert (jonbogert@gmail.com) v" << VER_NUM << std::endl;
        return 0;
    }

    Config config = Config::Load();
    if (config.timecardFile.empty())
    {
        Config::OpenPath();
        return 2;
    }

    if (strcmp(argv[1], "config") == 0)
    {
        Config::OpenPath();
        return 0;
    }

    Timecard timecard;
    timecard.Load(config.timecardFile);

    if (strcmp(argv[1], "period") == 0)
    {
        if (argc <= 2)
        {
            Help();
            return 1;
        }

        if (strcmp(argv[2], "list") == 0)
        {
            Op::PeriodList(timecard);
            return 0;
        }
        if (strcmp(argv[2], "set") == 0)
        {
            if (argc <= 3)
            {
                Help();
                return 1;
            }
            Op::PeriodSet(argv[3], timecard, config);
            return 0;
        }
        if (strcmp(argv[2], "show") == 0)
        {
            Op::PeriodShow(timecard, config.periodIndex);
            return 0;
        }
        if (strcmp(argv[2], "start") == 0)
        {
            Op::PeriodStart(timecard, config);
            return 0;
        }
        if (strcmp(argv[2], "calc") == 0)
        {
            Op::PeriodCalc(timecard, config, argc, argv);
            return 0;
        }
    }
    if (strcmp(argv[1], "export") == 0)
    {
        Op::ExportCSV(timecard, config);
        return 0;
    }

    if (strcmp(argv[1], "add") == 0)
    {
        Op::Add(timecard, config, argc, argv);
        return 0;
    }

    Help();

    return 1;
}
