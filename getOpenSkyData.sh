if [ $# -lt 1 ] || [ ! -n $1 ]; then
    echo 'Enter start time in unix timestamp. 1514764800 for Jan 1 2018'
    read startTime
else
    startTime=$1
fi
if [ $# -lt 2 ] || [ ! -n $2 ]; then
    echo 'Enter end time in unix timestamp. 1546300800 for Jan 1 2019'
    read endTime
else
    endTime=$2
fi
# Pull data from OpenSky
#                                                                                                                            | All US registered icao24 code begins with a
#                                                                                                                            |                             | Select N-callsigns or empty
#                                                                                                                            |                             |
script -f -c "echo 'select icao24, track.time, track.latitude, track.longitude from flights_data4, flights_data4.track WHERE (LEFT(Icao24, 1) = \"a\") AND (REGEXP_LIKE(Callsign, \"N\\\\d\\\\d.{0,3}\") OR Callsign is null OR Callsign = \"\") AND day >= $startTime AND day <= $endTime AND NOT track.onground;' | ssh -p 2230 -l fjch1997 data.opensky-network.org" data/openSkyTracks_${startTime}_to_${endTime}.txt
# Convert terminal output into csv file.
#cat log.txt | grep "^|.*" | sed -e 's/\s*|\s*/,/g' -e 's/^,\|,$//g' -e 's/NULL//g' | awk '!seen[$0]++' > openSkyTracks_${startTime}_to_${endTime}.csv