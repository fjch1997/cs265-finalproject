#include <fstream>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include <unordered_set>
#include <thread>
#include <future>
#include <vector>
#include "myHelpers.h"
#include "SunSet.h"
#include "openSkyParser.h"

using namespace std;

struct RunArguments
{
    string *fileName;
    int startTime;
    int endTime;
    Jf955FinalProject::OpenSkyParser *parser;
};

long run(RunArguments args);

int main(int argc, char *argv[])
{
    int startTime;
    int endTime;
    if (!Jf955FinalProject::getStartAndEndTime(&startTime, &endTime, argc, argv))
        return 1;
    vector<string> openSkyFileNames;
    if (argc < 4)
    {
        while (true)
        {

            string openSkyFileName;
            cout << "Enter file name to OpenSky Data file. Enter empty to stop. e.g. data/openSkyTracks_1514764800_to_1546300800.txt\n";
            getline(cin, openSkyFileName);
            if (openSkyFileName == "")
            {
                if (openSkyFileNames.size() == 0)
                    openSkyFileNames.push_back("data/openSkyTracks_1514764800_to_1546300800.txt");
                break;
            }
            openSkyFileNames.push_back(openSkyFileName);
        }
    }
    for (int i = 3; i < argc; i++)
    {
        openSkyFileNames.push_back(argv[i]);
    }

    Jf955FinalProject::OpenSkyParser parser;
    parser.loadAcceptableAircraftIcao24();
    auto length = openSkyFileNames.size();
    auto threads = new future<long>[length];
    for (int i = 0; i < length; i++)
    {
        RunArguments args;
        args.startTime = startTime;
        args.endTime = endTime;
        args.parser = &parser;
        args.fileName = &openSkyFileNames[i];
        threads[i] = std::async(run, args);
    }
    long totalHours = 0;
    for (int i = 0; i < length; i++)
    {
        totalHours += threads[i].get();
    }
    delete[] threads;
    cout << totalHours;
}

long run(RunArguments args)
{
    auto result = args.parser->getTotalFlightHours(*args.fileName, args.startTime, args.endTime);
    return result;
}