#include "Robot.h"
#include "Mine.h"
#include <iostream>
#include "RobotState.h"
#include "ExploreState.h"
#include "ForageState.h"
#include "ClusterState.h"
#include "PerformanceBed.h"
#include "BasicForagingState.h"

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
	
	 //Distance from sink
	 max_distance_from_sink = 0;
	 distance_from_sink = 0;
	 //Performance variable measures
	resetPerformanceMeasures();
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

	 max_distance_from_sink = 0;
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

	 max_distance_from_sink = 0;
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
	 max_distance_from_sink = 0;
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
	//track movement of robot
	if (tracker) {
		trackMovement();
	}

	switch (activity) {
		case (EXPLORE): explore();break;//if (!robotState) robotState = new ExploreState(this); robotState->doStep(); break;
		case (FORAGE) : forage(); break;//if (!robotState) robotState = new ForageState(this); robotState->doStep(); break;
		case (CLUSTER) : cluster();break;// if (!robotState) robotState = new ClusterState(this); robotState->doStep(); break;
		case (BASICFORAGE) : robotState->doStep(); break;
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

void Robot::resetPerformanceMeasures() {
	foraged = false;
	typeForaged = -1;
	moved = false;
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

bool Robot::isEmpty(int dirx, int diry ) {
	return mine->grid[pos.x + dirx][ pos.y+ diry] == EMPTY;
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
		//TODO: Possibly irrelevant line of code. Optimize
		destination.x += dir.x;
		destination.y += dir.y;
	} else if (activity == FORAGE) {
		destination.x -= dir.x;
		destination.y -= dir.y;
	}


	calculateDistanceFromSink();

	moved = true;
}

void Robot::calculateDistanceFromSink() {
	//Calculates distance from the sink
	if ( division == GOLD ) {
		//gold is on the left
		if (  pos.y >= mine->size.y/2 ) {
			distance_from_sink = t.distance( pos.x, 1, pos.y, mine->size.y/2 );
		} else {
			distance_from_sink = t.distance( pos.x, 1, pos.y, pos.y );
		}
	} else {
		if ( pos.y <= mine->size.y/2 ) {
			distance_from_sink = t.distance( pos.x, 1, pos.y, mine->size.y/2 );
		} else {
			distance_from_sink = t.distance( pos.x, 1, pos.y, pos.y );
		}
	}

	//Determines maximum distance from sink
	if ( distance_from_sink > max_distance_from_sink ) { max_distance_from_sink = distance_from_sink ; }
}

void Robot::calculateFoV() {

/*	int tried_positions;
	bool left = true;
	int addedPos = 0;

	//Temporary dir for rotation
	Coord left_dir = dir;
	Coord right_dir = dir;

	//Forward direction
	if ( validPos(pos.x + dir.x, pos.y + dir.y ) && isEmpty(dir.x,dir.y) ) {
		FoV[addedPos] = dir;
		addedPos++;
	}
	tried_positions++;

	while ( tried_positions < 8 && addedPos < 5 ) {

		if ( left_dir == right_dir ) {
			//add frotn or back
		} else {
			
			//generate both left & right versions. 
			Coord option[2];
			if ( left_dir.x == 0 || left_dir.y == 0) {
				//left
				option[0].x = left_dir.x + left_dir.y;
				option[0].y = left_dir.y + left_dir.x;
			} else if {
				
			}
				//right
				option[1].x = tmp_dir.x - tmp_dir.y;
				option[1].y = tmp_dir.y - tmp_dir.x;
			} else {
				//option 1
				option[0].x = tmp_dir.x;
				option[0].y = 0;

				//option 1
				option[1].x = 0;
				option[1].y = tmp_dir.y;
			}

		}


		//generate both left & right versions. 
		Coord option[2];
		if ( tmp_dir.x == 0 || tmp_dir.y == 0) {
			//left
			option[0].x = tmp_dir.x + tmp_dir.y;
			option[0].y = tmp_dir.y + tmp_dir.x;

			//right
			option[1].x = tmp_dir.x - tmp_dir.y;
			option[1].y = tmp_dir.y - tmp_dir.x;
		} else {
			//option 1
			option[0].x = tmp_dir.x;
			option[0].y = 0;

			//option 1
			option[1].x = 0;
			option[1].y = tmp_dir.y;
		}

	}

	//choose to go LEFT or RIGHT
	int d =  (t.randomClosed() >= 0.5 ) ? 1 : 0;
	dir = option[d];
}*/
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
		clusterLocation.x =  pos.x;
		clusterLocation.y =  pos.y;

		//set home as the destination
		destination.x = oldSinkPos.x;
		destination.y = oldSinkPos.y;
	} else if (state_counter == 0 && activity == FORAGE) {
		//set the location of the cluster
		clusterLocation.x =  pos.x;
		clusterLocation.y =  pos.y;

		destination.x = oldSinkPos.x;
		destination.y = oldSinkPos.y;
	}

	//increment state counter
	state_counter++;

	//need to use accumulated direction vector to guide homing instinct. 
	dir.x = sgm(oldSinkPos.x - pos.x);
	dir.y = sgm(oldSinkPos.y - pos.y);

	//check if valid move
	if (validMove()) {
		//make the move
		makeMove();

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
		
		if ( isHome() ) { 
			state_counter=0;

			if (activity == EXPLORE ) {
				state = RECRUITING;
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
	if ( division == GOLD ) {
		//gold is on the left
		return pos.y < mine->size.y/2;
	} else {
		return pos.y >= mine->size.y/2;
	}

	return true;
}

