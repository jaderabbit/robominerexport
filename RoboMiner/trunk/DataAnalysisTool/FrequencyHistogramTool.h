#pragma once

#include <iostream>
#include <string>
#include <map>

using namespace std;


struct Coord {
	int x;
	int y;

	bool operator==(const Coord &other) const {
		if ( x == other.x && y == other.y) {
			return true;
		}
		return false;
	}

	bool operator<(const Coord &other) const {
		if ( x < other.x && y <= other.y) {
			return true;
		}
		return false;
	}
};

class FrequencyHistogramTool
{
public:
	FrequencyHistogramTool(void);
	~FrequencyHistogramTool(void);

	//main method
	bool createFrequencyHistogramData( string input, string output);

	//call individual methods
	bool readFile( string input);
	bool writeFile( string output);

private:
	map<Coord,int> uniquePoints;
};

