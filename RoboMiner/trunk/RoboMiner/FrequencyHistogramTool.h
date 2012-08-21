#pragma once

#include <iostream>
#include <string>
#include <map>
#include "Robot.h"

using namespace std;

struct cmp_str
{
   bool operator()(Coord a, Coord b)
   {
     return ( a.x < b.x || a.y < b.y );
   }
};


class FrequencyHistogramTool
{
public:
	FrequencyHistogramTool(void);
	FrequencyHistogramTool( int grid_x, int grid_y );
	~FrequencyHistogramTool(void);

	//main method
	bool createFrequencyHistogramData( string input, string output);

	//call individual methods
	bool readFile( string input);
	bool writeFile( string output);

	//helper
	int search( Coord x );

private:
	vector<vector<int>> grid;
	vector<int> count;
};

