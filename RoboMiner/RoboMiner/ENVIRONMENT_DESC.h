#pragma once
#include <string>

using namespace std;

struct ENVIRONMENT_DESC {
	string type;
	int num_objects;
	double ratio_gold;
	int grid_size;

	int sink_boundary;
};