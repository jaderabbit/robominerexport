#include "FrequencyHistogramTool.h"
#include <fstream>

FrequencyHistogramTool::FrequencyHistogramTool(void)
{
}


FrequencyHistogramTool::~FrequencyHistogramTool(void)
{
}

bool FrequencyHistogramTool::createFrequencyHistogramData( string input, string output) {
	uniquePoints.clear();
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

	while (in.good() ) {
		in >> n.x;
		in.ignore(1);
		in >> n.y;

		if (uniquePoints.find(n) == uniquePoints.end()) { //if new
			uniquePoints.insert(pair<Coord,int>(n,0));
		} else {
			uniquePoints[n]  = uniquePoints[n] + 1;
		}
	}

	in.close();
	return true;
}

bool FrequencyHistogramTool::writeFile( string output) {
	ofstream out(output);

	map<Coord,int>::iterator it;
	for (it = uniquePoints.begin(); it != uniquePoints.end(); it++ ) {
		out << "(" << (*it).first.x << ":" << (*it).first.y << ")" << " ," << (*it).second << endl;
	}

	out.close();
	return true;
}

