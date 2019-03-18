build : getNumberOfFatalities getTotalFlightHours test

run-test : test
	./test

test : src/tests.cc src/myHelpers.cc src/SunSet.cpp src/SunSet.h src/openSkyParser.h src/openSkyParser.cc
	g++ -o test src/tests.cc src/openSkyParser.cc src/SunSet.cpp src/myHelpers.cc

getNumberOfFatalities : src/getNumberOfFatalities.cc src/HTTPDownloader.hpp src/HTTPDownloader.cpp src/myHelpers.cc src/SunSet.cpp src/SunSet.h src/openSkyParser.h src/openSkyParser.cc
	g++ -o getNumberOfFatalities src/HTTPDownloader.cpp src/getNumberOfFatalities.cc src/myHelpers.cc src/SunSet.cpp src/openSkyParser.cc -Iinclude/curl -lcurl

getTotalFlightHours : src/getTotalFlightHours.cc src/myHelpers.cc src/SunSet.cpp src/openSkyParser.h src/openSkyParser.cc
	g++ -o getTotalFlightHours src/SunSet.cpp src/getTotalFlightHours.cc src/myHelpers.cc src/openSkyParser.cc -pthread

clean :
	rm getNumberOfFatalities getTotalFlightHours test
