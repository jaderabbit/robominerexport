#include "Mine.h"
#include <iostream>
#include <fstream>
#include <sstream>

Mine::Mine(void)
{
}

Mine::Mine(Tools &_t) : t(_t)
{
}

Mine::~Mine(void)
{
	grid.clear();
}

Mine::Mine(int x, int y, int num_rob, int num_items, int num_item_types) {
	//init size
	size.x = x;
	size.y = y;

	//initialize sink
	sink_items[GOLD-1] = 0; 
	sink_items[WASTE-1] = 0; 

	//initialize sink positions
	//goldSinkBoundaries[0]

	//init grid world
	for (int i=0; i < x; i++) {
		vector<Block> t;
		for (int j=0; j < y; j++) {
			Block b;
			b.index = -1;
			b.type = EMPTY;
			t.push_back(b);
		}
		grid.push_back(t);
	}

	//init Sink
	initSink();

	//init objects
	initObjects(num_items,num_item_types);

	//init robots
	initRandomPositionRobots(num_rob);
}

bool Mine::load( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc, string fileName ) {
	//initialize sink
	sink_items[GOLD-1] = 0; 
	sink_items[WASTE-1] = 0; 

	//init size
	size.x = _env_desc.grid_size;
	size.y = _env_desc.grid_size;

	//read in file
	fileInput(fileName);

	//Set the sink and Wipe sink boundary area
	//initSink();

	return true;
}

//initialization grid
void Mine::initializeEmptyGrid( int x, int y ) {
	//init size
	size.x = x;
	size.y = y;

	//init grid world
	for (int i=0; i < x; i++) {
		vector<Block> t;
		for (int j=0; j < y; j++) {
			Block b;
			b.index = -1;
			b.type = EMPTY;
			t.push_back(b);
		}
		grid.push_back(t);
	}
}

//get cells and set cells
bool Mine::setCell( int x, int y, int value, int index ) {
	if ( isValid(x,y) ) {
		grid[x][y].type = value;
		if (index > -1 ) grid[x][y].index = index;
		return true;
	}
	return false;
}

bool Mine::setCellIfEmpty( int x, int y, int value, int index ) {
	if ( isEmpty(x,y) ) {
		grid[x][y].type = value;
		if (index > -1 ) grid[x][y].index = index;
		return true;
	}
	return false;
}

int Mine::getCell( int x, int y, int value ) {
	if ( isValid(x,y) ) return value;
	else return -1;
}

bool Mine::isValid( int x, int y) {
	if ( x > 0 && x < size.x && y > 0 && y < size.y ) {
		return true;
	}
	return false;
}

bool Mine::isEmpty( int x, int y) {
	if ( isValid(x,y) && grid[x][y] == EMPTY ) {
		return true;
	}
	return false;
}

//cell index
void Mine::setCellIndex( int x, int y, int index ) {
	if (isValid(x,y)) grid[x][y].index = index;
}

int Mine::getCellIndex( int x, int y) {
	if (isValid(x,y)) return grid[x][y].index;
	else return -1;
}

bool Mine::load(int x, int y, int num_rob, int ratio_rob, string inputFile, int algorithm) {
	//reset
	grid.clear();
	robots.clear();

	//initialize sink
	sink_items[GOLD-1] = 0; 
	sink_items[WASTE-1] = 0; 

	//init size
	size.x = x;
	size.y = y;

	//read in file
	bool success = fileInput(inputFile);

	//init robots
	initHomeRobots(num_rob, ratio_rob);

	//algorithm
	alg = algorithm;

	return success;
}

Mine::Mine(int x, int y, int num_rob, int ratio_rob, string inputFile) {
	//initialize sink
	sink_items[GOLD-1] = 0; 
	sink_items[WASTE-1] = 0; 

	//init size
	size.x = x;
	size.y = y;

	//read in file
	fileInput(inputFile);

	//init robots
	initHomeRobots(num_rob, ratio_rob);
}

void Mine::initSink() {
	//init sink
	for (int i=0; i < size.y; i++) {
		if ( i < size.y/2)
			grid[0][i].type = G_SINK;
		else {
			grid[0][i].type = W_SINK;
		}
	}

	//wipe the sink boundary
	for (int i=1; i < SINK_BOUNDARY; i++) {
		for (int j=0; j < size.y; j++) {
			grid[i][j].type = EMPTY;
		}
	}
}

void Mine::initRandomPositionRobots( int num ) {
	for (int i=0; i < num; i ++ ) {
		bool empty = false;
		while (!empty) {
			//generate random position.
			Coord p;
			p.x= t.random(0,size.x-1);
			p.y = t.random(0,size.y-1);

			if ( grid[p.x][p.y] == EMPTY) {
				//generate random direction
				Coord d;
				d.x = t.random(-1,1);
				d.y = t.random(-1,1);
		
				//set position & index on board
				grid[p.x][p.y].type = ROBOT;
				grid[p.x][p.y].index = i;
				
				//determine what the miner initially is mining
				int division = (i < num/2) ? GOLD : WASTE;

				//Construct robot
				Robot r(&(*this),p,d,CLUSTER,UNLOADED,t.random(0,MAX_WANDER_STEPS), division);

				//stop loop
				empty =  true;

				//push back robot
				robots.push_back(r);		
			}
		}
	}
}

void Mine::initHomeRobots( int num, int ratio_rob) {
	for (int i=0; i < num ; i ++ ) {
		//choose position
		Coord p;

		//generate position located close to sink
		do { 
			int range = num/size.x + 1;
			p.x = t.random(0,range); 
			p.y = t.random(0,size.y-1);
		} while (grid[p.x][p.y].type != EMPTY);

		//p.x = i/size.x + 1; p.y = i;
			
		//generate random direction
		Coord d;
		d.x = t.random(-1,1);
		d.y = t.random(-1,1);
		
		//set position & index on board
		grid[p.x][p.y].type = ROBOT;
		grid[p.x][p.y].index = i;
				
		//determine what the miner initially is mining - set a 3rd of them to explire
		int activity = (i < num/ratio_rob) ? EXPLORE : FORAGE;

		//string stream for file name
		stringstream s;
		s << "tracker_robot_" << i << ".csv";

		
		//determine what the miner initially is mining
		int division = (i%2==0) ? GOLD : WASTE;

		//Construct robot
		Robot r(&(*this),p,d,activity,t.random(1,MAX_WANDER_STEPS),division,s.str().c_str());

		//push back robot
		robots.push_back(r);

	}
}

void Mine::initObjects( int num_objects, int num_item_types) {
	//Initialization of objects - Even amount of waste and gold	
	//initialize waste randomly 
	for (int i=0; i < num_objects/2; i++) {
		bool empty = false;
		while (!empty) {
			//generate random position.
			int x_new = t.random(5,size.x-1);
			int y_new = t.random(0,size.y-1);

			if ( grid[x_new][y_new] == EMPTY) {
				grid[x_new][y_new].type = WASTE;
				empty = true;
			}
		}
	}

	//initialize gold randomly;
	for (int i=0; i < num_objects/2; i++) {
		bool empty = false;
		while (!empty) {
			//generate random position.
			int x_new = t.random(5,size.x-1);
			int y_new = t.random(0,size.y-1);

			if ( grid[x_new][y_new] == EMPTY) {
				grid[x_new][y_new].type = GOLD;
				empty = true;
			}
		}
	}
}

void Mine::output() {
	int gold = 0;
	int waste = 0;
	int robots = 0;
	int sink_num = 0;
	for (int j=0; j < size.y; j++) {
		for (int i=0; i < size.x; i++) {
			char a =' ';
			switch( grid[i][j].type ) {
				case 0: { a = '.'; break;}
				case 1: {a = 'G'; gold++; break;}
				case 2: {a = 'W'; waste++; break;}
				case 3: {a = 'R';robots++; break;}
				case 4: {a = 'S'; break;}
				case 5: {a = 'g'; break;}
				case 6: {a = 'w'; break;}
				default: cout << "invalid type" << endl;
			}
			cout << a << " ";
		}
		cout << endl;
	}
	cout << "Num Robots=" << robots << "; Num Gold=" << gold << "; Num Waste=" << waste << "; Sink_items_gold=" << sink_items[0] <<"; Sink_items_waste=" << sink_items[1] << endl;
	cout << endl;
}

void Mine::outputAdvanced() {
	int gold = 0;
	int waste = 0;
	int robs = 0;
	int sink_num = 0;
	for (int j=0; j < size.y; j++) {
		for (int i=0; i < size.x; i++) {
			char a=' ';
			switch( grid[i][j].type ) {
				case 0: { a = '.'; break;}
				case 1: {a = 'G'; gold++; break;}
				case 2: {a = 'W'; waste++; break;}
				case 3: {a = ('a' + grid[i][j].index);robs++; break;}
				case 4: {a = 'S'; break;}
				case 5: {a = 'g'; break;}
				case 6: {a = 'w'; break;}
				default: cout << "invalid type" << endl;
			}
			cout << a << " ";
		}
		cout << endl;
	}
	cout << "Num sink=" << sink_num << " Num Robots=" << robs << "; Num Gold=" << gold << "; Num Waste=" << waste << "; Sink_items_gold=" << sink_items[0] <<"; Sink_items_waste=" << sink_items[1] << endl;
	cout << "Key----------------------------" << endl;
	for (unsigned int i=0; i < robots.size(); i++) {
		char a =  ('a' + i) ;
		cout << i << " = " << a<< endl;
	}
	cout << endl;
}

void Mine::outputAdvanced(int option) {
	int gold = 0;
	int waste = 0;
	int robs = 0;
	int sink_num = 0;
	for (int j=0; j < size.y; j++) {
		for (int i=0; i < size.x; i++) {
			char a=' ';
			switch( grid[i][j].type ) {
				case 0: { a = '.'; break;}
				case 1: {a = 'G'; gold++; break;}
				case 2: {a = 'W'; waste++; break;}
				case 3: {a = determineCharacter(option, grid[i][j].index);robs++; break;}
				case 4: {a = 'S'; break;}
				case 5: {a = 'g'; break;}
				case 6: {a = 'w'; break;}
				default: cout << "invalid type" << endl;
			}
			cout << a << " ";
		}
		cout << endl;
	}
	cout << "Num Robots=" << robs << "; Num Gold=" << gold << "; Num Waste=" << waste << "; Sink_items_gold=" << sink_items[0] <<"; Sink_items_waste=" << sink_items[1] << endl;
}

char Mine::determineCharacter( int option, int robot_index) {
	switch (option) {
		case STATE_OUTPUT: return char(robots[robot_index].state + '0');
		case INDEX_OUTPUT: return char(robot_index + 'a');
		case ACTIVITY_OUTPUT: return char(robots[robot_index].activity + '0');
		case LOADED_OUTPUT: return (robots[robot_index].loaded) ? 'L' : 'U';
		default: return 'R'; break;
	}
}

void Mine::fileOutput(string fname) {
	ofstream f; 
	f.open(fname.c_str(),ios_base::app);

	int gold = 0;
	int waste = 0;
	int robots = 0;
	int sink_num = 0;
	for (int j=0; j < size.y; j++) {
		for (int i=0; i < size.x; i++) {
			char a=' ';
			switch( grid[i][j].type ) {
				case 0: { a = '.'; break;}
				case 1: {a = 'G'; gold++; break;}
				case 2: {a = 'W'; waste++; break;}
				case 3: {a = 'R';robots++; break;}
				case 4: {a = 'S'; sink_num += (grid[i][j].type-4); break;}
				case 5: {a = 'g'; sink_num += (grid[i][j].type-4); break;}
				case 6: {a = 'w'; sink_num += (grid[i][j].type-4); break;}
				default: cout << "invalid type" << endl;
			}
			f << a << " ";
		}
		f << endl;
	}
	f << "Num Robots=" << robots << "; Num Gold=" << gold << "; Num Waste=" << waste << "; Sink_num=" << sink_num << endl;
	f << endl;
	f.close();
}

bool Mine::fileInput(string fname) {

	//input file
	ifstream f;
	f.open(fname.c_str(),ios_base::app);

	if (!f.is_open()) return false;

	//clear grid
	grid.clear();

	//initialize counters
	int gold = 0;
	int waste = 0;
	int robots = 0;
	int sink_num = 0;

	//initialize empty grid
	initializeEmptyGrid(size.x, size.y);

	//take input from file
	for (int i=0; i < size.y; i++) {
		for (int j=0; j < size.x; j++) {
			//read in type 
			char a = 0;
			int type = 0;
			f >> a;

			//determine type
			switch( a ) {
				case '.': { type = EMPTY; break;}
				case 'G': {type = GOLD; gold++; break;}
				case 'W': {type = WASTE; waste++; break;}
				case 'R': {type = ROBOT; robots++; break;}
				case 'g': {type = G_SINK; sink_num++; break;}
				case 'w': {type = W_SINK;  sink_num++; break;}
			}
			
			//create and save block
			Block b; b.index = -1; b.type = type;
			grid[j][i] = b;
		}

	}

	f.close();
	return true;
}

void Mine::robotTrackerOutput() {
	for (unsigned int i=0; i < robots.size(); i++) {
		robots[i].trackerOutput();
	}
}

void Mine::antCemeteryAlgorithm() {

	//output world before
	output();
	fileOutput("before.txt");

	//set activity
	for (unsigned int i=0; i < robots.size(); i++) {
		robots[i].setActivity(CLUSTER);
	}

	//run algorithm
	for (unsigned int i=0; i < MAX_ITERATIONS; i++) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}

		if ( i % 200 == 0 ) {
			//cout << "ITERATION " << i << endl;
			//fileOutput("intermediate_step.txt");
		}
	}

	//output world after
	output();
	fileOutput("after.txt");
}

void Mine::recruitmentAlgorithm() {
	
	//output world before
	output();
	fileOutput("before.txt");

	//run algorithm
	for (unsigned int i=0; i < MAX_ITERATIONS; i++) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}

		if ( i < 50) {
			//cout << "ITERATION " << i << endl;
			fileOutput("intermediate_step.txt");
		}
	}

	//output world after
	output();
	fileOutput("after.txt");
	
}

void Mine::algorithmStep() {

}

void Mine::recruitmentAlgorithmStep() {
	if (cnt < MAX_ITERATIONS) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}
}

void Mine::antCemetaryAlgorithmStep() {
	
	//set activity
	for (unsigned int i=0; i < robots.size(); i++) {
		robots[i].setActivity(CLUSTER);
	}

	if (cnt < MAX_ITERATIONS) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}
}

void Mine::foragingAlgorithmStep() {
	//set activity
	for (unsigned int i=0; i < robots.size(); i++) {
		robots[i].setActivity(CLUSTER);
	}

	if (cnt < MAX_ITERATIONS) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}
}