#include "FrequencyHistogramTool.h"
#include <sstream>


int main() {
	cout << "Enter number of files" << endl;
	int num=0;
	cin >> num;
	FrequencyHistogramTool f;

	for (int i=0; i < num; i++) {
		stringstream sIn;
		stringstream sOut;
		sIn << "tracker_robot_" << i << ".csv";
		sOut << "histogram_robot" << i << ".csv";
		f.createFrequencyHistogramData(sIn.str(),sOut.str());
	}
}
