#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <ctime>
#include <unordered_set>
#include "SunSet.h"
#include "myHelpers.h"

using namespace std;

namespace Jf955FinalProject
{
bool getStartAndEndTime(int *startTime, int *endTime, int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Enter start time in unix timestamp. 1514764800 for Jan 1 2018.\n";
        string response;
        getline(cin, response);
        if (response == "")
            *startTime = 1514764800;
        else if (!stoi(response, startTime, "Invalid start time.\n"))
            return false;
    }
    else
    {
        if (!stoi(argv[1], startTime, "Invalid start time.\n"))
            return false;
    }
    if (argc < 3)
    {
        cout << "Enter end time in unix timestamp. 1546300800 for Jan 1 2019.\n";
        string response;
        getline(cin, response);
        if (response == "")
            *endTime = 1546300800;
        else if (!stoi(response, endTime, "Invalid end time.\n"))
            return false;
    }
    else
    {
        if (!stoi(argv[2], endTime, "Invalid end time.\n"))
            return false;
    }
    return true;
}

bool stoi(string str, int *result, string errorMessage)
{
    try
    {
        *result = stoi(str);
        return true;
    }
    catch (const std::exception &e)
    {
        cerr << errorMessage;
        return false;
    }
}
string getNext(string *line, int *previous)
{
    int current = line->find('|', *previous);
    int p = *previous;
    *previous = current + 2;
    return line->substr(p, current - p - 1);
}
} // namespace Jf955FinalProject