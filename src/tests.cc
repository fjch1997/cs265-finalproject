#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "myHelpers.h"
#include "SunSet.h"
#include <time.h>
#include "openSkyParser.h"

using namespace Jf955FinalProject;

TEST_CASE("isNight", "[openSkyParser]")
{
    tm time;
    time.tm_isdst = false;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_wday = 0;
    time.tm_yday = 0;
    time.tm_year = 2018 - 1900;
    time.tm_mon = 11;
    time.tm_mday = 7;
    time.tm_hour = 6;
    time.tm_gmtoff = 0;
    SunSet sunSet;
    Jf955FinalProject::OpenSkyParser parser;
    REQUIRE(parser.isNight(mktime(&time) - timezone, &sunSet, -46.6253, -23.5338) == true);
    REQUIRE(parser.isNight(mktime(&time) - timezone, &sunSet, 115.857, -31.9535) == false);
    time.tm_hour = 4;
    REQUIRE(parser.isNight(mktime(&time) - timezone, &sunSet, -46.6253, -23.5338) == true);
    REQUIRE(parser.isNight(mktime(&time) - timezone, &sunSet, 115.857, -31.9535) == false);
    time.tm_hour = 22;
    REQUIRE(parser.isNight(mktime(&time) - timezone, &sunSet, -46.6253, -23.5338) == true);
    REQUIRE(parser.isNight(mktime(&time) - timezone, &sunSet, 115.857, -31.9535) == true);
    time.tm_hour = 21;
    REQUIRE(parser.isNight(mktime(&time) - timezone, &sunSet, -46.6253, -23.5338) == false);
    REQUIRE(parser.isNight(mktime(&time) - timezone, &sunSet, 115.857, -31.9535) == true);
    time.tm_hour = 23;
    time.tm_min = 15;
    REQUIRE(parser.isNight(mktime(&time) - timezone, &sunSet, -96.74555053710935, 33.19610595703125) == false);
    time.tm_hour = 23;
    time.tm_min = 45;
    REQUIRE(parser.isNight(mktime(&time) - timezone, &sunSet, -96.74555053710935, 33.19610595703125) == true);
}

TEST_CASE("getNext", "[myHelpers]")
{
    string line = "20190311X01519 | Accident | GAA19CA164 | 03/11/2019 | Kalispell, MT | United States |  |  | GPI |  | Non-Fatal | Substantial | Airplane | N723M | Maule | M7 | No | 1 |  | Part 91: General Aviation |  | Personal |  |  |  |  | 2 |  |  | Preliminary | 03/12/2019 | ";
    int previous = 0;
    REQUIRE(getNext(&line, &previous) == "20190311X01519");
    REQUIRE(getNext(&line, &previous) == "Accident");
    REQUIRE(getNext(&line, &previous) == "GAA19CA164");
    REQUIRE(getNext(&line, &previous) == "03/11/2019");
    REQUIRE(getNext(&line, &previous) == "Kalispell, MT");
    REQUIRE(getNext(&line, &previous) == "United States");
    REQUIRE(getNext(&line, &previous) == "");
    REQUIRE(getNext(&line, &previous) == "");
    REQUIRE(getNext(&line, &previous) == "GPI");
    REQUIRE(getNext(&line, &previous) == "");
    REQUIRE(getNext(&line, &previous) == "Non-Fatal");
}

TEST_CASE("getTotalFlightHours", "[openSkyParser]")
{
    Jf955FinalProject::OpenSkyParser parser;
    parser.loadAcceptableAircraftIcao24();
    auto hours = parser.getTotalFlightHours("data/sample_openSkyTracks.txt", 1517167536, 1517290006);
    // hours should be 1086
    REQUIRE(hours < 1357); // 25% margin of error due to slightly different sunset/sunrise time algorithm.
    REQUIRE(hours > 814);
}