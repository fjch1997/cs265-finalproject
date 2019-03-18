#include <fstream>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include <unordered_set>
#include <thread>
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
    long *totalHours;
    Jf955FinalProject::OpenSkyParser *parser;
};

void run(RunArguments args);

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
    long totalHours = 0;

    auto threads = new thread *[openSkyFileNames.size()];

    for (int i = 0; i < openSkyFileNames.size(); i++)
    {
        RunArguments args;
        args.startTime = startTime;
        args.endTime = endTime;
        args.totalHours = &totalHours;
        args.parser = &parser;
        args.fileName = &openSkyFileNames[i];
        threads[i] = new thread(run, args);
    }
    for (int i = 0; i < openSkyFileNames.size(); i++)
    {
        threads[i]->join();
        delete threads[i];
    }
    delete threads;
    cout << totalHours;
}

void run(RunArguments args)
{
    long hours = args.parser->getTotalFlightHours(*(args.fileName), args.startTime, args.endTime);
    *(args.totalHours) += hours;
}