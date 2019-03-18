#include <fstream>
#include <string>
#include <algorithm>
#include "SunSet.h"
#include "openSkyParser.h"
#include "myHelpers.h"

using namespace std;
using namespace Jf955FinalProject;

namespace Jf955FinalProject
{

OpenSkyParser::OpenSkyParser()
{
}

OpenSkyParser::~OpenSkyParser()
{
}

void OpenSkyParser::loadAcceptableAircraftIcao24()
{
    ifstream file;
    file.open("data/FAA_Registry_2017.txt");

    string line;
    getline(file, line);
    while (getline(file, line))
    {
        int typeOfEngine = stoi(line.substr(250, 2));
        if (typeOfEngine != 1)
            continue;
        string code = line.substr(601, 6);
        transform(code.begin(), code.end(), code.begin(), ::tolower);
        this->codes.insert(code);
    }
}
long OpenSkyParser::getTotalFlightHours(string openSkyFileName, int startTime, int endTime)
{
    // OpenSky Data
    ifstream file;
    file.open(openSkyFileName);
    string line;
    getline(file, line);
    SunSet sunset;
    long totalTime = 0;
    string previousIcao24 = "";
    time_t previousTrackTime = 0;
    while (getline(file, line))
    {
        if (line.find("| a") != 0)
            continue; // This line is not a data entry.
        int previous = 2;
        auto icao24 = getNext(&line, &previous);
        if (this->codes.find(icao24) == this->codes.end())
            continue; // Not our type of aircraft.
        auto timeStr = getNext(&line, &previous);
        time_t timeLong = stol(timeStr);
        if (timeLong < startTime || timeLong > endTime)
            continue;
        double lat = stod(getNext(&line, &previous));
        double lon = stod(getNext(&line, &previous));
        if (!this->isNight(timeLong, &sunset, lon, lat))
            continue;
        if (previousIcao24 == icao24 && previousTrackTime != 0)
        {
            if (previousTrackTime < timeLong)
            {
                // A break in continous block of records.
                totalTime += 30;
            }
            totalTime += (previousTrackTime - timeLong);
        }
        else if (previousIcao24 != "" && previousTrackTime != 0)
        {
            totalTime += 120; // Assume an average of 2 minutes below ADS-B receiption altitude.
        }
        previousIcao24 = icao24;
        previousTrackTime = timeLong;
    }
    return totalTime;
}
bool OpenSkyParser::isNight(time_t timeLong, SunSet *sunset, double lon, double lat)
{
    tm *time = gmtime(&timeLong);
    string utcTime = asctime(time);
    sunset->setCurrentDate(time->tm_year, time->tm_mon, time->tm_mday);
    sunset->setPosition(lat, lon, 0);
    auto sunriseMinutesSinceUtcMidnight = sunset->calcSunriseUTC();
    auto sunsetMinutesSinceUtcMidnight = sunset->calcSunsetUTC();
    time->tm_hour = 0;
    time->tm_min = 0;
    time->tm_sec = 0;
    auto startOfDay = mktime(time) - timezone;
    auto timeMinutesSinceUtcMidnight = (timeLong - startOfDay) / 60;
    return timeMinutesSinceUtcMidnight < sunriseMinutesSinceUtcMidnight || timeMinutesSinceUtcMidnight > sunsetMinutesSinceUtcMidnight ;
}
} // namespace Jf955FinalProject