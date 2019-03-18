make getNumberOfFatalities
make getTotalFlightHours

echo "Enter start time in unix timestamp. 1514764800 for Jan 1 2018"
read startTime
echo "Enter end time in unix timestamp. 1546300800 for Jan 1 2019"
read endTime

echo "If you already have the number of fatalities, enter it here. If not, press enter to download."
read response
if [[ $response =~ ^[0-9]+$ ]]; then
  numFatalities=$response
else
  echo "Downloading NTSB data to get the number of fatalities. This will take around 15 minutes per year."
  numFatalities=$(./getNumberOfFatalities $startTime $endTime)
fi

echo "There are $numFatalities fatalities in the given time period."

echo "Enter file path to OpenSky data for the given time period. These files can be downloaded by running getOpenSkyData.sh (Takes sbout 5 hours for a month of data). Seperate multiple files with space. Add quotes if necessary. Multiple files will be run in parallel."
read files

echo "Analyzing OpenSky data to get total night flight hours."
totalHours=$( ./getTotalFlightHours $startTime $endTime $files)

echo "Total hours flown is $totalHours"
if [ ! $totalHours -eq 0 ]; then
  rate=$(echo "$numFatalities / $totalHours * 60 * 60 * 100000" | bc -l)
  echo "This gives a fatality rate of $rate per 100,000 hours."
fi