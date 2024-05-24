#ifndef include_util_h_
#define include_util_h_

#include <array>
#include <string>
#include <vector>

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace util
{

const std::array<std::string, 7> days_of_week = {"Sunday", "Monday", "Tuesday",
    "Wednesday", "Thursday", "Friday", "Saturday"
};
const std::vector<std::string> hours = {"00", "01", "02", "03", "04", "05", "06",
    "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
    "20", "21", "22", "23"
};

static int get_day(const std::string& date_str, std::string& day, int day_offset=0)
{
    if (date_str.empty())
    {
        return 1;
    }
    std::tm tm = {};
    std::istringstream ss(date_str);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M");
    if (ss.fail())
    {
        std::cerr << "Failed to parse date string " << date_str << std::endl;
        return 1;
    }

    std::time_t time = std::mktime(&tm);
    if (time == -1)
    {
        std::cerr << "Failed to convert to time_t" << date_str << std::endl;
        return 1;
    }

    // Apply day offset
    time += day_offset * 24 * 60 * 60;

    std::tm* time_tm = std::localtime(&time);
    day = util::days_of_week[time_tm->tm_wday];
    return 0;
}

}  // namespace util

#endif  // include_util_h_
