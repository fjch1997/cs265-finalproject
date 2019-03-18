#ifndef OPEN_SKY_PARSER_H
#define OPEN_SKY_PARSER_H
#include <stdio.h>
#include <string>
#include <unordered_set>
#include "SunSet.h"

namespace Jf955FinalProject
{
class OpenSkyParser
{
private:
  /* data */
  std::unordered_set<std::string> codes;

public:
  OpenSkyParser(/* args */);
  ~OpenSkyParser();
  void loadAcceptableAircraftIcao24();
  long getTotalFlightHours(std::string openSkyFileName, int startTime, int endTime);
  bool isNight(time_t timeLong, SunSet *sunset, double lon, double lat);
};
} // namespace Jf955FinalProject
#endif