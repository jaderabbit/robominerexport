#include <iostream>
#include <string>
#include "../Helper/Tools.h"
#include <sstream>
#include <fstream>

using namespace std;
using namespace RoboMiner;

//Parameters
const int num_grid_sizes = 5;
const int num_percentage_objects = 5;
const int num_gold_ratios = 9;
const int num_sims = 30;
const int sink_boundary = 5;

int grid_sizes[num_grid_sizes] = { 50, 100, 200, 300, 500 };
int percentage_objects[num_percentage_objects] = { 5, 20, 50, 70, 90 };
double gold_ratios[num_gold_ratios] = { 0, 0.2, 0.25, 0.33333333, 0.5, 0.666666667, 0.75, 0.8, 1 };

vector<vector<int>> initializeGrid( int _grid_size ) {
	vector<vector<int>> grid;

	//init grid world
	for (int i=0; i < _grid_size; i++) {
		vector<int> t;
		for (int j=0; j < _grid_size; j++) {
			t.push_back(EMPTY);
		}
		grid.push_back(t);
	}

	return grid;
}

void outputFile( string fname, vector<vector<int>> grid ) {
	ofstream f; 
	f.open(fname.c_str(),ios_base::app);

	int gold = 0;
	int waste = 0;
	int robots = 0;
	int sink_num = 0;
	for (int j=0; j < grid.size(); j++) {
		for (int i=0; i < grid.size(); i++) {
			char a;
			switch( grid[i][j] ) {
				case 0: { a = '.'; break;}
				case 1: {a = 'G'; break;}
				case 2: {a = 'W'; break;}
				case 3: {a = 'R'; break;}
				case 4: {a = 'S'; break;}
				case 5: {a = 'g'; break;}
				case 6: {a = 'w'; break;}
				default: cout << "invalid type" << endl;
			}
			f << a << " ";
		}
		f << endl;
	}
	//f << "Num Robots=" << robots << "; Num Gold=" << gold << "; Num Waste=" << waste << "; Sink_num=" << sink_num << endl;
	f << endl;
}

string createFileName( string _env, int _num_objects, double _ratio_gold, int _grid_size, int _sim_num ) {

	//file name
	stringstream fname;
	fname <<  _env << "\\" <<_env << "_size_" << _grid_size << "_obj_" << _num_objects << "_ratio_" << _ratio_gold << "_sim_" << _sim_num <<".txt";
	return fname.str();
}

vector<vector<int>> uniformDistribution(int _num_objects, double _ratio_gold, int _grid_size, Tools &t, int SINK_BOUNDARY ) {
	//initialize grid of specified grid size
	vector<vector<int>> grid = initializeGrid(_grid_size);

	//uniformly distribute the amount of objects of required ratio
	//Randomly
	for (int i=0; i < _num_objects; i++) {
		bool empty = false;
		while (!empty) {
			//generate random position.
			int x_new = t.random(SINK_BOUNDARY,_grid_size-1);
			int y_new = t.random(0,_grid_size-1);

			//set new position
			if ( i < (_num_objects*_ratio_gold ) )
				if (grid[x_new][y_new] == EMPTY )  {
					grid[x_new][y_new] = GOLD;
					empty = true;
				} 
			else {
				if (grid[x_new][y_new] == EMPTY )  {
					grid[x_new][y_new] = WASTE;
					empty = true;
				} 
			}
		}
	}

	//return grid
	return grid;
}

void generateUniformDistributionEnvironments() {
	Tools t;
	//for 5 grid sizes
	//for 5 object distributions
	//for 9 gold to waste distributions. 
	//for 30 sims each
	for (int i=0; i < num_grid_sizes; i++ ) {
		for (int j=0; j < num_percentage_objects; j++ ) {
			for (int k=0; k < num_gold_ratios; k++) {
				for (int l=0; l < num_sims; l++ ) {
					//create name
					string name = createFileName("uniform",percentage_objects[j]*grid_sizes[i]*grid_sizes[i],gold_ratios[k],grid_sizes[i],l);

					//create environment
					vector<vector<int>> grid = uniformDistribution(percentage_objects[j]*grid_sizes[i]*grid_sizes[i],gold_ratios[k],grid_sizes[i],t,sink_boundary);

					//output file
					outputFile( name, grid );
				}
			}
		}
	}

}

void generateGaussianEnvironments() {
}

void generateVeinEnvironments() {
}

void generateClusteredEnvironments() {

}

int main() {
	//30 of each
	generateUniformDistributionEnvironments();
	generateGaussianEnvironments();
	generateClusteredEnvironments();
	generateVeinEnvironments();
}