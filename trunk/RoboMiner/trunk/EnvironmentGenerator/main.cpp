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
	f.open(fname);

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
	f.close();
}

string createFileName( string _env, int _num_objects, double _ratio_gold, int _grid_size, int _sim_num ) {

	//file name
	stringstream fname;
	fname << "environments/" <<  _env << "/" <<_env << "_size_" << _grid_size << "_obj_" << _num_objects << "_ratio_" << _ratio_gold << "_sim_" << _sim_num <<".txt";
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
			if ( i < (_num_objects*_ratio_gold ) ) {
				if (grid[x_new][y_new] == EMPTY )  {
					grid[x_new][y_new] = GOLD;
					empty = true;
				} 
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

vector<vector<int>> gaussianDistribution(int _num_objects, double _ratio_gold, int _grid_size, Tools &t, int SINK_BOUNDARY) {
	//initialize grid of specified grid size
	vector<vector<int>> grid = initializeGrid(_grid_size);

	//get center
	int center_x = SINK_BOUNDARY+((_grid_size-SINK_BOUNDARY)/2);
	int center_y = _grid_size/2;

	//gaussian
	for (int i=0; i < _ratio_gold*_num_objects; i++) {

		bool empty = false;
		while (!empty) {
			//Choose type
			int x_new = t.gaussianDistribution(center_x, (_grid_size-SINK_BOUNDARY)/2*_ratio_gold);
			int y_new = t.gaussianDistribution(center_y, _grid_size/2*_ratio_gold);

			if ( x_new > 0 && x_new < _grid_size && y_new > 0 && y_new < _grid_size ) {
				//check if new position is empty. If so place
				if (grid[x_new][y_new] == EMPTY )  {
						grid[x_new][y_new] = GOLD;
						empty = true;
				} 
			}
		}
	}

	for (int i =  _ratio_gold*_num_objects; i < _num_objects; i++) {
		bool empty = false;
		while (!empty) {

			//generate random position.
			int x_new = t.random(SINK_BOUNDARY,_grid_size-1);
			int y_new = t.random(0,_grid_size-1);

			//check if new position is empty. If so place
			if (grid[x_new][y_new] == EMPTY )  {
					grid[x_new][y_new] = WASTE;
					empty = true;
			} 
		}
	}

	return grid;

}

vector<vector<int>> veinDistribution(int _num_objects, double _ratio_gold, int _grid_size, Tools &t, int SINK_BOUNDARY) {

	//Generate Gold
	int sides[4][2]= { {0,1} , {1,0}, {1,60}, {60,1} };

	//initialize grid of specified grid size
	vector<vector<int>> grid = initializeGrid(_grid_size);

	//Randomly choose line points
	int lowerbound = _grid_size/3;
	int upperbound = 2*lowerbound;

	//2 random numbers
	int a1 = t.random(lowerbound,upperbound);
	int b1 = t.random(lowerbound,upperbound);

	//Choose 2 random sides
	Coord a, b;
	int side1 = t.random(0,4); 
	int side2 = side1;
	while (side1 == side2 ) {
		side2 = t.random(0,4); 
	}

	//Generate coordinate
	a.x = ( sides[side1][0] == 0 || sides[side1][0] == 60 ) ?  sides[side1][0] : a1;
	a.y = ( sides[side1][1] == 0 || sides[side1][1] == 60 ) ?  sides[side1][1] : a1;

	b.x = ( sides[side2][0] == 0 || sides[side2][0] == 60 ) ?  sides[side2][0] : b1;
	b.y = ( sides[side2][1] == 0 || sides[side2][1] == 60 ) ?  sides[side2][1] : b1;

	//Generate line
	int k = abs(a.x - b.x);

	//deviation
	int deviation = 1;

	//If NOT undefined slope
	if ( k > 0 ) {
		//Calculate M
		double m = (1.0*(a.y - b.y ))/(1.0*(a.x - b.x));

		//Calculate C
		int c = a.y - m*a.x;

		//Boundaries
		int xbounds[2];
		xbounds[0] = min(a.x, b.x);
		xbounds[1] = max(a.x,b.x);

		//Generate Points
		for (int i=0; i < _ratio_gold*_num_objects; i++) {

			bool empty = false;
			int count = 0;
			while (!empty) {

				//Choose x as part of valid domain
				int x_new = t.random(xbounds[0],xbounds[1]-1);
				int y_new = (int) m*x_new + c;

				//Add gassian noise to it
				x_new = t.gaussianDistribution(x_new,deviation);
				y_new = t.gaussianDistribution(y_new,deviation);

				//Check if valid position
				if ( x_new >= 0 && x_new < _grid_size && y_new >= 0 && y_new < _grid_size) {
					//check if new position is empty. If so place
					if (grid[x_new][y_new] == EMPTY )  {
							grid[x_new][y_new] = GOLD;
							empty = true;							
					} 
				}
			}
		}
	} else { //k=0
		//Generate Points
		//Generate Points
		for (int i=0; i < _ratio_gold*_num_objects; i++) {

			bool empty = false;
			int count = 0;
			while (!empty) {

				//Add guassian noise to a.x to get x_new
				int x_new = t.gaussianDistribution(a.x,deviation);

				//Randomly choose y_new on valid domain
				int y_new = t.random(0,_grid_size-1);


				//Check if valid position
				if ( x_new >= 0 && x_new < _grid_size && y_new >= 0 && y_new < _grid_size) {
					//check if new position is empty. If so place
					if (grid[x_new][y_new] == EMPTY )  {
							grid[x_new][y_new] = GOLD;
							empty = true;							
					} 
				}
			}

	}
	
	//Generate Waste
	for (int i =  _ratio_gold*_num_objects; i < _num_objects; i++) {
		bool empty = false;
		while (!empty) {

			//generate random position.
			int x_new = t.random(SINK_BOUNDARY,_grid_size-1);
			int y_new = t.random(0,_grid_size-1);

			//check if new position is empty. If so place
			if (grid[x_new][y_new] == EMPTY )  {
					grid[x_new][y_new] = WASTE;
					empty = true;
			} 
		}
	}

	return grid;
}

vector<vector<int>> clusteredDistribution(int _num_objects, double _ratio_gold, int _grid_size, Tools &t, int SINK_BOUNDARY) {
	//initialize grid of specified grid size
	vector<vector<int>> grid = initializeGrid(_grid_size);

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
					string name = createFileName("uniform",(0.01)*percentage_objects[j]*grid_sizes[i]*grid_sizes[i],gold_ratios[k],grid_sizes[i],l);

					//create environment
					vector<vector<int>> grid = uniformDistribution((0.01)*percentage_objects[j]*grid_sizes[i]*grid_sizes[i],gold_ratios[k],grid_sizes[i],t,sink_boundary);

					//output file
					outputFile( name, grid );
				}
			}
		}
	}

}

void generateGaussianEnvironments() {
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
					string name = createFileName("gaussian",(0.01)*percentage_objects[j]*grid_sizes[i]*grid_sizes[i],gold_ratios[k],grid_sizes[i],l);

					//create environment
					vector<vector<int>> grid = gaussianDistribution((0.01)*percentage_objects[j]*grid_sizes[i]*grid_sizes[i],gold_ratios[k],grid_sizes[i],t,sink_boundary);

					//output file
					outputFile( name, grid );
				}
			}
		}
	}
}

void generateVeinEnvironments() {

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
					string name = createFileName("vein",(0.01)*percentage_objects[j]*grid_sizes[i]*grid_sizes[i],gold_ratios[k],grid_sizes[i],l);

					//create environment
					vector<vector<int>> grid = veinDistribution((0.01)*percentage_objects[j]*grid_sizes[i]*grid_sizes[i],gold_ratios[k],grid_sizes[i],t,sink_boundary);

					//output file
					outputFile( name, grid );
				}
			}
		}
	}
}

void generateClusteredEnvironments() {
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
					string name = createFileName("clustered",(0.01)*percentage_objects[j]*grid_sizes[i]*grid_sizes[i],gold_ratios[k],grid_sizes[i],l);

					//create environment
					vector<vector<int>> grid = uniformDistribution((0.01)*percentage_objects[j]*grid_sizes[i]*grid_sizes[i],gold_ratios[k],grid_sizes[i],t,sink_boundary);

					//output file
					outputFile( name, grid );
				}
			}
		}
	}

}

int main() {

	//generateUniformDistributionEnvironments();
	//generateGaussianEnvironments();
	generateVeinEnvironments();
	generateClusteredEnvironments();
}