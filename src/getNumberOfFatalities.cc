#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <ctime>
#include "HTTPDownloader.hpp"
#include "myHelpers.h"

using namespace std;
using namespace Jf955FinalProject;

int main(int argc, char *argv[])
{
    int startTime;
    int endTime;
    
    if (!getStartAndEndTime(&startTime, &endTime, argc, argv))
        return 1;
    ifstream inFile;
    inFile.open("data/NTSB_Since_1962.txt");
    string line;
    getline(inFile, line);
    int totalFatalities = 0;
    while (getline(inFile, line))
    {
        try
        {
            int previous = 0;
            string eventId = getNext(&line, &previous);
            getNext(&line, &previous);
            getNext(&line, &previous);
            string dateStr = getNext(&line, &previous);
            // tm dateTm;
            // dateTm.tm_mon = stoi(dateStr.substr(0, 2));
            // dateTm.tm_mday = stoi(dateStr.substr(3, 2));
            // dateTm.tm_year = stoi(dateStr.substr(6, 4)) - 1900;
            struct tm tm;
            // Filter by date.
            if (strptime(dateStr.c_str(), "%m/%d/%Y", &tm) == NULL)
                continue;
            auto time = (long)mktime(&tm);
            if ((long)time < startTime || (long)time > endTime)
                continue;
            for (int i = 0; i < 6; i++)
            { // Skip columns
                getNext(&line, &previous);
            }
            // Filter by injury
            auto injury = getNext(&line, &previous);
            if (injury.find("Fatal") != 0)
                continue;
            auto endIndex = injury.find(')');
            auto numberOfFatalities = stoi(injury.substr(6, endIndex - 1));
            for (int i = 0; i < 6; i++)
            { // Skip columns
                getNext(&line, &previous);
            }
            // Filter by Number of engines.
            auto numberOfEngines = getNext(&line, &previous);
            if (numberOfEngines != "" && numberOfEngines != "1")
                continue;
            // Filter by Engine Type.
            auto engineType = getNext(&line, &previous);
            if (engineType != "" && engineType != "Reciprocating")
                continue;
            // FAR
            auto far = getNext(&line, &previous);
            if (far != "" && far != "Part 91: General Aviation")
                continue;
            HTTPDownloader downloader;
            auto html = downloader.download("https://app.ntsb.gov/pdfgenerator/ReportGeneratorFile.ashx?EventID=" + eventId + "&AKey=1&RType=HTML&IType=LA");
            if (html.find("<span style=\"font-family:'Trebuchet MS'\">Night/Dark</span>") == string::npos && html.find("<span style=\"font-family:'Trebuchet MS'\">Night</span>") == string::npos)
                continue;
            totalFatalities += numberOfFatalities;
        }
        catch (exception)
        {
            cerr << "Bad data: " << line << "\n";
        }
    }
    cout << totalFatalities;
    return 0;
}
