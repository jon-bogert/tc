
#include <iostream>
#include <string>

void Help()
{
    std::string contents = R"(TimeCard -- Help

Usage: tc <task> <flags>

Tasks:
    config                      | opens config folder
================================|===============================================================
    add <start> <end> "Note"    | Add an entry to the current period on today's date
================================|===============================================================
    period set <index|"latest"> | set period to index
    period list                 | list all periods with date ranges
    period start                | starts a new period on today's date
    period show                 | shows all entries in current period
    period calc                 | calculates current period time total and amount
================================|===============================================================
    export                      | exports data to a CSV file with the same path as timecard data

Times can be entered in the following format:
    16.5    | 24-hour decimal time (also without decimal)
    16:30   | 24-hour with minute
    4.5p    | 12-hr decimal time (also without decimal)
    4:30p   | 12-hr with minutes

)";

    std::cout << contents;
}