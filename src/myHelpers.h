#ifndef MY_HELPERS_H
#define MY_HELPERS_H
#include <string>

using namespace std;

namespace Jf955FinalProject
{
bool stoi(string str, int *result, string errorMessage);
string getNext(string *line, int *previous);
bool getStartAndEndTime(int *startTime, int *endTime, int argc, char *argv[]);
} // namespace Jf955FinalProject
#endif