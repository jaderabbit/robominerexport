#include "Mine.h"
#include <iostream>
#include <fstream>
#include <sstream>

Mine::Mine(void)
{
	//init size
	size.x = 80;
	size.y = 80;

	//init grid world
	for (int i=0; i < size.x; i++) {
		vector<Block> t;
		for (int j=0; j < size.y; j++) {
			Block b;
			b.index = -1;
			b.type = EMPTY;
			t.push_back(b);
		}
		grid.push_back(t);
	}

	//output blank file
	fileOutput("single.txt");
}

Mine::~Mine(void)
{
}

Mine::Mine(int x, int y, int num_rob, int num_items, int num_item_types) {
	//init size
	size.x = x;
	size.y = y;

	//initialize sink
	sink_items[GOLD-1] = 0; 
	sink_items[WASTE-1] = 0; 

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

bool Mine::load(int x, int y, int num_rob, int ratio_rob, string inputFile) {
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
		grid[0][i].type = SINK;
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
			char a;
			switch( grid[i][j].type ) {
				case 0: { a = '.'; break;}
				case 1: {a = 'G'; gold++; break;}
				case 2: {a = 'W'; waste++; break;}
				case 3: {a = 'R';robots++; break;}
				default: {a = 'S'; sink_num += (grid[i][j].type-4); break;}
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
			char a;
			switch( grid[i][j].type ) {
				case 0: { a = '.'; break;}
				case 1: {a = 'G'; gold++; break;}
				case 2: {a = 'W'; waste++; break;}
				case 3: {a = ('a' + grid[i][j].index);robs++; break;}
				default: {a = 'S'; sink_num += (grid[i][j].type-3); break;}
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
			char a;
			switch( grid[i][j].type ) {
				case 0: { a = '.'; break;}
				case 1: {a = 'G'; gold++; break;}
				case 2: {a = 'W'; waste++; break;}
				case 3: {a = determineCharacter(option, grid[i][j].index);robs++; break;}
				default: {a = 'S'; sink_num += (grid[i][j].type-4); break;}
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
			char a;
			switch( grid[i][j].type ) {
				case 0: { a = '.'; break;}
				case 1: {a = 'G'; gold++; break;}
				case 2: {a = 'W'; waste++; break;}
				case 3: {a = 'R';robots++; break;}
				default: {a = 'S'; sink_num += (grid[i][j].type-4); break;}
			}
			f << a << " ";
		}
		f << endl;
	}
	f << "Num Robots=" << robots << "; Num Gold=" << gold << "; Num Waste=" << waste << "; Sink_num=" << sink_num << endl;
	f << endl;

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

	//take input from file
	for (int i=0; i < size.x; i++) {
		vector<Block> row;
		for (int j=0; j < size.y; j++) {
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
				default: {type = SINK; sink_num++; break;}
			}
			
			//create and save block
			Block b; b.index = -1; b.type = type;
			row.push_back(b);
		}

		//create grid
		grid.push_back(row);
	}

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

void Mine::recruitmentAlgorithmStep() {
	if (cnt < MAX_ITERATIONS) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}
}