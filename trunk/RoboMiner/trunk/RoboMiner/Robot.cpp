#include "Robot.h"
#include "Mine.h"
#include <iostream>
#include "RobotState.h"
#include "ExploreState.h"
#include "ForageState.h"
#include "ClusterState.h"

Robot::Robot(void)
{
	pos.x = 0; pos.y = 0;		//position
	dir.x = 0; dir.y = 0;		//direction

	activity = -1;	//activity
	state = -1;		//state

	//Load
	load_type = -1;
	loaded = false;	//perhaps unnecessary

	//Random wander
	max_path = 50;
	path_count = 0;

	//Repition counter
	activity_counter = 0;
	state_counter = 0;

	//Ant Cemetery Building
	 f = 0;

	 //World
	 mine = 0;

	 //home vector
	 homeVector.x = 0; homeVector.y = 0;
}

Robot::Robot(Mine* _mine) : mine(_mine) {
	
	//Load
	load_type = -1;
	loaded = false;	//perhaps unnecessary
	
	//Repition counter
	state_counter = 0;

	//Ant Cemetery Building
	 f = 0;

	 //Set initial states
	 init_states[CLUSTER] = UNLOADED;
	 init_states[FORAGE] = WAITING;
	 init_states[EXPLORE] = EXPLORING;

	 //Initial path length
	 chooseMaxPathLength();	
	 
	 //Recruiter Message and directions
	 homeVector.x = 0; homeVector.y = 0;
	 clusterLocation.x = 0; clusterLocation.y = 0;
	 destination.x = 0; destination.y = 0;
	 recruiterOriginalPos.x = 0; recruiterOriginalPos.y = 0;
}

Robot::Robot( Mine* _mine, Coord _pos, Coord _dir, int _act, int _state, int _max_path, int _div,  string track_file){
	mine = _mine;
	pos = _pos;
	dir = _dir;
	activity = _act;
	state = _state;
	max_path = _max_path;
	division = _div;

	//Load
	load_type = -1;
	loaded = false;	//perhaps unnecessary
	
	//Repition counter
	state_counter = 0;

	//Ant Cemetery Building
	 f = 0;

	 //Set initial states
	 init_states[CLUSTER] = UNLOADED;
	 init_states[FORAGE] = WAITING;
	 init_states[EXPLORE] = EXPLORING;

	 //Initial path length
	 chooseMaxPathLength();	
	 
	 //Recruiter Message and directions
	 homeVector.x = 0; homeVector.y = 0;
	 clusterLocation.x = 0; clusterLocation.y = 0;
	 destination.x = 0; destination.y = 0;
	 recruiterOriginalPos.x = 0; recruiterOriginalPos.y = 0;
}

Robot::Robot( Mine* _mine, Coord _pos, Coord _dir, int _act, int _max_path, string track_file){
	mine = _mine;
	pos = _pos;
	dir = _dir;
	max_path = _max_path;

	//Set initial states
	init_states[CLUSTER] = UNLOADED;
	init_states[FORAGE] = WAITING;
	init_states[EXPLORE] = EXPLORING;

	//MaxActivity Counters
	max_activity_counters[CLUSTER] = 10;
	max_activity_counters[FORAGE] = 10;
	max_activity_counters[EXPLORE] = 10;

	//Activity
	activity = _act;
	state = init_states[_act];

	//Default Division
	division = GOLD;

	//Load
	load_type = -1;
	loaded = false;	//perhaps unnecessary
	
	//Repition counter
	activity_counter = 0;
	state_counter = 0;

	//Ant Cemetery Building
	 f = 0;

	 //Initial path length
	 chooseMaxPathLength();	

	 //Recruiter Message and directions
	 homeVector.x = 0; homeVector.y = 0;
	 clusterLocation.x = 0; clusterLocation.y = 0;
	 destination.x = 0; destination.y = 0;
	 recruiterOriginalPos.x = 0; recruiterOriginalPos.y = 0;

	 //Tracker
	 if (track_file.length() > 0 ) {
		tracker = true;
		trackFile = track_file;
	 }
}

Robot::Robot( Mine* _mine, Coord _pos, Coord _dir, int _act, int _max_path, int _div, string track_file){
	mine = _mine;
	pos = _pos;
	dir = _dir;
	max_path = _max_path;

	//Set initial states
	init_states[CLUSTER] = UNLOADED;
	init_states[FORAGE] = WAITING;
	init_states[EXPLORE] = EXPLORING;

	//MaxActivity Counters
	max_activity_counters[CLUSTER] = 10;
	max_activity_counters[FORAGE] = 10;
	max_activity_counters[EXPLORE] = 10;

	//Activity
	activity = _act;
	state = init_states[_act];

	//Default Division
	division = _div;

	//Load
	load_type = -1;
	loaded = false;	//perhaps unnecessary
	
	//Repition counter
	activity_counter = 0;
	state_counter = 0;

	//Ant Cemetery Building
	 f = 0;

	 //Initial path length
	 chooseMaxPathLength();	

	 //Recruiter Message and directions
	 homeVector.x = 0; homeVector.y = 0;
	 clusterLocation.x = 0; clusterLocation.y = 0;
	 destination.x = 0; destination.y = 0;
	 recruiterOriginalPos.x = 0; recruiterOriginalPos.y = 0;

	 //Tracker
	 if (track_file.length() > 0 ) {
		tracker = true;
		trackFile = track_file;
	 }

	 robotState = 0;
}

Robot::~Robot(void)
{
}

void Robot::trackerOutput() {
	//write tracker to file
	ofstream out(trackFile.c_str());
	out << "x,y" << endl;
	for (int i=0; i < track.size(); i++) {
		out << track[i].x << "," << track[i].y << endl;
	}
	out.close();
}

void Robot::setActivity( int _act) {
	activity = _act;
	state = init_states[activity];
}

void Robot::doStep() {
	if (tracker) {
		trackMovement();
	}

	switch (activity) {
		case (EXPLORE): explore();//if (!robotState) robotState = new ExploreState(this); robotState->doStep(); break;
		case (FORAGE) : forage(); //if (!robotState) robotState = new ForageState(this); robotState->doStep(); break;
		case (CLUSTER) : cluster();// if (!robotState) robotState = new ClusterState(this); robotState->doStep(); break;
		default: cout << "Erroneous ACTIVITY" << endl; break;
	}
}

void Robot::chooseActivity() {
	activity_counter++;
	
	if (activity_counter < max_activity_counters[activity] ) {
		state_counter = 0;
		state = init_states[activity];
	} else {
		activity_counter = 0;
		state_counter = 0;
		activity = ( activity + 1 ) % 3;
		state = init_states[activity];
	}

}

bool Robot::validMove() {
	if ( (pos.x + dir.x >= 0) && (pos.x + dir.x < mine->size.x)  && (pos.y + dir.y < mine->size.y) && (pos.y + dir.y >= 0)) {
		if (mine->grid[pos.x + dir.x][pos.y + dir.y] == EMPTY ) {		
			return true;
		} 
		return false;
	}
	return false;
}

bool Robot::walkingIntoAWall(){
	if ( (pos.x + dir.x >= 0) && (pos.x + dir.x < mine->size.x)  && (pos.y + dir.y < mine->size.y) && (pos.y + dir.y >= 0)) {
		return false;
	}
	return true;
}

void Robot::makeMove() {
	mine->grid[pos.x][pos.y].type = EMPTY; //prev pos to empty 
	mine->grid[pos.x + dir.x][pos.y + dir.y].type = ROBOT; //current pos to robot
	mine->grid[pos.x + dir.x][pos.y + dir.y].index = mine->grid[pos.x][pos.y].index;
	pos.x += dir.x;
	pos.y += dir.y;

	if (activity == EXPLORE) {
		destination.x += dir.x;
		destination.y += dir.y;
	} else if (activity == FORAGE) {
		destination.x -= dir.x;
		destination.y -= dir.y;
	}
}

void Robot::setPosition( int x, int y) { 
	mine->grid[pos.x][pos.y].type = EMPTY; //prev pos to empty 
	mine->grid[x][y].type = ROBOT; //current pos to robot
	mine->grid[x][y].index = mine->grid[pos.x][pos.y].index;

	pos.x = x; pos.y = y;

	reset();
}

void Robot::homingStep() {
	//set destination
	if (state_counter == 0 && activity == EXPLORE ) {	
		//set the location of the cluster
		clusterLocation.x =  destination.x;
		clusterLocation.y =  destination.y;

		//set home vector
		homeVector.x = -destination.x;
		homeVector.y = -destination.y;

		//set home as the destination
		destination.x = homeVector.x;
		destination.y = homeVector.y;
	} else if (state_counter == 0 && activity == FORAGE) {
		//clusterLocation.x = pos.x;
		//clusterLocation.y = pos.y;

		destination.x = homeVector.x;
		destination.y = homeVector.y;
	}

	//increment state counter
	state_counter++;

	//need to use accumulated direction vector to guide homing instinct. 
	dir.x = sgm(destination.x);
	dir.y = sgm(destination.y);

	//check if valid move
	if (validMove()) {
		//make the move
		makeMove();

		//if home  - not exactly correct is it?
		/*28/06/2012 14:42*/
		if ( isHome() ) { //this clause makes sure it is home
			state_counter = 0;

			//not very good. Need to improve at some point when cluster *RUNS OUT*
			if (activity == EXPLORE) {
				state = RECRUITING; //has to be home at this point
			}
			else if (activity == FORAGE) {
				state = UNLOADING;
			}
		}
	//if move is not valid - switch to beacon homing
	} else {
		state = BEACON_HOMING;
		state_counter = 0;
	}
}

void Robot::beaconHomingStep() {
	bool success = false;
	int rep_count = 0;

	//get direction home - HACK - cuz wer
	dir.x = sgm(0 - pos.x);
	if ( load_type == GOLD ) {
		//gold is on the left
		if (  pos.y >= mine->size.y/2 ) {
			dir.y = -1;
		} else {
			dir.y = 0;
		}
	} else {
		if ( pos.y <= mine->size.y/2 ) {
			dir.y = 1;
		} else {
			dir.y = 0;
		}
	}

	do {
		//This hack is a LIE. this is a weird hack to check if the robot is home - need to also ensure that robot is in ACTIVITY = RECRUITING
		if ( isHome() ) { //this was wrong. 
			state_counter=0;

			if (activity == EXPLORE ) {
				state = RECRUITING;
				//mine->output();
			} else if (activity == FORAGE) {
				state = UNLOADING;
			}
			success = true;
			break;
		}
		//move is valid
		if (validMove() ) {
			//make move
			makeMove();

			//successfully moved
			success = true;

		} else {
			//if something in the way avoid obstacle
			avoidObstacle();
			rep_count++;
		}
	} while (!success && rep_count < 8); //rep count < 8 to try all sides

	//increment state counter
	state_counter++;
}

void Robot::reset() {
	if (state == EXPLORING) {
		destination.x = 0; destination.y = 0;
		clusterLocation.x = 0; clusterLocation.y = 0;
		homeVector.x = 0; homeVector.y = 0;
		state_counter = 0;
	}
}

int  Robot::sgm(int x) {
	return ((x > 0) - (x < 0));
}

bool Robot::isHome() {
	//return (pos.x == 1);
	return (pos.x == 1 && directionYToSink());
}

bool Robot::directionYToSink() {
	if ( load_type == GOLD ) {
		//gold is on the left
		return pos.y < mine->size.y/2;
	} else {
		return pos.y >= mine->size.y/2;
	}

	return true;
}

