#include "FrequencyHistogramTool.h"
#include <fstream>
#include <assert.h>

FrequencyHistogramTool::FrequencyHistogramTool(void)
{
}


FrequencyHistogramTool::~FrequencyHistogramTool(void)
{
}

FrequencyHistogramTool::FrequencyHistogramTool( int grid_x, int grid_y ) {
	for (int i=0; i < grid_x; i++) {
		vector<int> row;
		for (int j=0; j < grid_y; j++) {
			row.push_back(0);
		}
		grid.push_back(row);
	}
}

bool FrequencyHistogramTool::createFrequencyHistogramData( string input, string output) {
	if (readFile(input) && writeFile(output) ) {
		return true;
	}
	return false;
}

bool FrequencyHistogramTool::readFile( string input) {

	ifstream in(input);
	Coord n;
	string header;
	in >> header;

	if ( in.is_open() ) {
		while (in.good() ) {
			in >> n.x;
			in.ignore(1);
			in >> n.y;

			grid[n.x][n.y]++;
		}
	} else {
		cout << "could not open file: " << input << endl;
	}

	in.close();
	return true;
}

bool FrequencyHistogramTool::writeFile( string output) {
	ofstream out(output);
	if (out.is_open() ) {
		for (unsigned int i=0; i < grid.size(); i++) {
			for (unsigned int j=0; j < grid[0].size(); j++) {
				out << grid[i][j] << ",";
			}
			out << endl;
		}
		out << endl;
	} else {
		cout << "could not open file: " << output << endl;
	}
	out.close();
	return true;
}

