all:
	g++ -std=c++17 -Wall -O3 -o RVC *.cpp
test:
	g++ -std=c++17 -Wall -o ParserTests tests/ParserTestCases.cpp Parser.cpp && ./ParserTests --success
