build : getNumberOfFatalities getTotalFlightHours test FAA_Registry_2017

FAA_Registry_2017 : data/FAA_Registry_2017.zip
	unzip data/FAA_Registry_2017.zip -d data

run-test : test
	./test

test : src/tests.cc src/myHelpers.cc src/SunSet.cpp src/SunSet.h src/openSkyParser.h src/openSkyParser.cc
	g++ -o test src/tests.cc src/openSkyParser.cc src/SunSet.cpp src/myHelpers.cc --std=c++0x

getNumberOfFatalities : src/getNumberOfFatalities.cc src/HTTPDownloader.hpp src/HTTPDownloader.cpp src/myHelpers.cc src/SunSet.cpp src/SunSet.h src/openSkyParser.h src/openSkyParser.cc
	g++ -o getNumberOfFatalities src/HTTPDownloader.cpp src/getNumberOfFatalities.cc src/myHelpers.cc src/SunSet.cpp src/openSkyParser.cc -Iinclude/curl -lcurl --std=c++0x

getTotalFlightHours : src/getTotalFlightHours.cc src/myHelpers.cc src/SunSet.cpp src/openSkyParser.h src/openSkyParser.cc
	g++ -o getTotalFlightHours src/SunSet.cpp src/getTotalFlightHours.cc src/myHelpers.cc src/openSkyParser.cc -pthread --std=c++0x

clean :
	rm getNumberOfFatalities getTotalFlightHours test data/FAA_Registry_2017.txt
