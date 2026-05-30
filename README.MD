# Time Card by Jon "Xepherin" Bogert (jonbogert@gmail.com) -- README

A command line timecard management application

# Usage

As shown with `--help` command

```
Usage: tc <task> <flags>

Tasks:
    config                                   | opens config folder
=============================================|===============================================================
    add <start> <end> "Note"                 | Add an entry to the current period on today's date
=============================================|===============================================================
    period set <index|"latest">              | set period to index (negative index counts back from latest)
    period list                              | list all periods with date ranges
    period start                             | starts a new period on today's date
    period show                              | shows all entries in current period
    period calc                              | calculates current period time total and amount
      <-p|--proj> <startDate> <endDate>      | Project dates onto provided date range ('YYYY-MM-DD' format)
      <-r|--range> <startIndex> <opt. count> | Calculates only the specified range of entries
=============================================|===============================================================
    export                                   | exports data to a CSV file with the same path as timecard data

Times can be entered in the following format:
    16.5    | 24-hour decimal time (also without decimal)
    16:30   | 24-hour with minute
    4.5p    | 12-hr decimal time (also without decimal)
    4:30p   | 12-hr with minutes
```

# Dependancies
The following libraries are required. Static libraries are included for Visual Studio on Windows
  - [yaml-cpp](https://github.com/jbeder/yaml-cpp) by jbeder
  - [CPP-DateTime-library](https://github.com/jeremydumais/CPP-DateTime-library) by jeremydumais (source code)

# Repository Info

This repository uses the [premake5](https://premake.github.io/) build system. Execute `build-vs2022.bat` to generate a Visual Studio 2022 solution. This project currently only supports Windows.
