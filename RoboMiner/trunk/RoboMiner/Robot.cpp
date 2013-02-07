#include "Robot.h"
#include "Mine.h"
#include <iostream>
#include "RobotState.h"
#include "ExploreState.h"
#include "ForageState.h"
#include "ClusterState.h"
#include "PerformanceBed.h"
#include "BasicForagingState.h"
#include <assert.h>

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

	//Direction circle
	dir_circle[0].x = 1; dir_circle[0].y = 0;
	dir_circle[1].x = 1; dir_circle[1].y = 1;
	dir_circle[2].x = 0; dir_circle[2].y = 1;
	dir_circle[3].x = -1; dir_circle[3].y = 1;
	dir_circle[4].x = -1; dir_circle[4].y = 0;
	dir_circle[5].x = -1; dir_circle[5].y = -1;
	dir_circle[6].x = 0; dir_circle[6].y = -1;
	dir_circle[7].x = 1; dir_circle[7].y = -1;

	lambda = 0.2;

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
	if ( validPos( pos.x + dirx, pos.y + diry ) )
		return mine->grid[pos.x + dirx][ pos.y+ diry] == EMPTY;
	return false;
}

bool Robot::isEmpty(Coord p) {
	if ( validPos( p.x, p.y ) ) {
		bool tmp =  mine->grid[p.x][p.y].type == EMPTY;
		return tmp;
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
	if (validMove()){
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

double Robot::calculateDistanceFromSink(Coord new_dir) {
	Coord new_pos;
	new_pos.x = pos.x + new_dir.x;
	new_pos.y = pos.y + new_dir.y;

	//Calculates distance from the sink
	if ( division == GOLD ) {
		//gold is on the left
		if ( pos.y >= mine->size.y/2 ) {
			return t.distance( new_pos.x, 1, new_pos.y, mine->size.y/2 );
		} else {
			return t.distance( new_pos.x, 1, new_pos.y, new_pos.y );
		}
	} else {
		if ( pos.y <= mine->size.y/2 ) {
			return t.distance( new_pos.x, 1, new_pos.y, mine->size.y/2 );
		} else {
			return t.distance( new_pos.x, 1, new_pos.y, new_pos.y );
		}
	}
}

double Robot::calculateDistanceFromLocation(Coord new_dir) {
	Coord new_pos;
	new_pos.x = pos.x + new_dir.x;
	new_pos.y = pos.y + new_dir.y;

	//Calculates distance from the location
	return t.distance( new_pos.x, clusterLocation.x, new_pos.y, clusterLocation.y );

}

int Robot::getDirectionIndex( Coord c ) {
	for (int i=0; i < 8; i++) {
		if ( c.x == dir_circle[i].x && c.y == dir_circle[i].y ) {
			return i;
		}
	}
}

int Robot::calculateFoV() {
	//get index of current direction
	int left, right, left_start, right_start;
	left_start = right_start = left = right = getDirectionIndex(dir);
	int curr = 0;
	int i = 1;

	//add current direction to list
	if ( validPos(pos.x + dir.x, pos.y + dir.y) &&  isEmpty(dir.x, dir.y ) ) {
		FoV[curr] = dir;
		curr++;
	}

	//iteration using method through directions and add if available, until all spots are used or you run out
	while ( curr < 5 && i < 8/2 ) {
		left = ( left_start + i + 8 ) % 8;
		right = ( right_start - i + 8 ) % 8;
		
		//randomly choose LEFT or RIGHT
		if (t.randomOpen() < 0.5 ) {
			if ( validPos(pos.x + dir_circle[left].x, pos.y + dir_circle[left].y) &&  isEmpty(dir_circle[left].x, dir_circle[left].y) ) {
				FoV[curr] = dir_circle[left];
				assert( validPos(pos.x + FoV[curr].x, pos.y + FoV[curr].y) );
				curr++;
			}
			if (validPos(pos.x + dir_circle[right].x, pos.y + dir_circle[right].y) &&  isEmpty(dir_circle[right].x, dir_circle[right].y) && curr < 5) {
				FoV[curr] = dir_circle[right];
				assert( validPos(pos.x + FoV[curr].x, pos.y + FoV[curr].y) );
				curr++;
			}
		} else {
			if (validPos(pos.x + dir_circle[right].x, pos.y + dir_circle[right].y) &&  isEmpty(dir_circle[right].x, dir_circle[right].y) ) {
				FoV[curr] = dir_circle[right];
				assert( validPos(pos.x +FoV[curr].x, pos.y + FoV[curr].y) );
				curr++;
			}
			if ( validPos(pos.x + dir_circle[left].x, pos.y + dir_circle[left].y) &&  isEmpty(dir_circle[left].x, dir_circle[left].y) && curr < 5) {
				FoV[curr] = dir_circle[left];
				assert( validPos(pos.x + FoV[curr].x, pos.y + FoV[curr].y) );
				curr++;
			}
		}
		

		i++;
	}

	//FoV now contains a list of valid directions, returns number of valid dir in FoV
	return curr;
}

double Robot::calculateClarity(Coord d) {
	double u_max = DoV;
	double u =0;
	double beta;
	Coord tmp_pos; tmp_pos.x = pos.x; tmp_pos.y = pos.y;

	//Run through depth of view
	for (int i=0; i < DoV; i++ ) {
		//calc new pos
		tmp_pos.x += d.x;
		tmp_pos.y += d.y;

		//if empty inc u else break out of loop
		if (isEmpty(tmp_pos)) {
			u += 1.0;
		} else {
			break;
		}
	}

	//beta = clarity
	beta = (u_max - u)/u_max;

	return beta;
}

double Robot::calculateDesirability( Coord d , double rank , double max_rank ){

	double alpha = 0; 
	alpha = rank/max_rank;

	//calculate distance
	/*double alpha = 0;
	if ( state == BEACON_HOMING ) 
		alpha = calculateDistanceFromSink(d)/max_distance_from_sink;
	else 
		alpha = calculateDistanceFromLocation(d)/max_distance_from_sink;
		*/

	//calculate clarity
	double beta = calculateClarity(d);

	double delta=0;
	//combine & return
	if ( pos.x < 4 ) 
		delta = alpha;
	else
		delta = lambda*alpha + (1-lambda)*beta;

	//assert( delta >= 0 && delta <= 1 );
	return delta;
}

void  Robot::chooseForagerDirection() {
	//choose initial direction
	dir = directionToSink();

	//calculate FoV
	int num = calculateFoV();

	//calc max distance
	//double dist[5];
	//double max_distance = 0;
	//for (int i=1; i < num; i++) {
	//	dist[i] = calculateDistanceFromSink(FoV[i]);
	//}

	//t.sortConcurrent(dist,FoV,num);

	//calculate desirability
	int min_desirability_index = 0;
	double min_desirability = calculateDesirability(FoV[0],0,num); //need to minimize
	for (int i=1; i < num; i++) {
		assert( validPos(pos.x + FoV[i].x,pos.y + FoV[i].y) );
		double des = calculateDesirability(FoV[i],i,num);
		if ( des < min_desirability ) {
			min_desirability_index = i;
			min_desirability = des;
		}
	}

	//direction is thus
	dir = FoV[min_desirability_index];
	//assert( validPos(pos.x + dir.x,pos.y + dir.y) );
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
	//check if home
	if ( isHome() ) { 
		state_counter=0;

		if (activity == EXPLORE ) {
			state = RECRUITING;
		} else if (activity == FORAGE) {
			state = UNLOADING;
		}
	}

	//New method
	chooseForagerDirection();

	//make move
	makeMove();

	/*bool success = false;
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
	} while (!success && rep_count < 8); //rep count < 8 to try all sides*/

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
		return pos.y <= mine->size.y/2;
	} else {
		return pos.y >= mine->size.y/2;
	}

	return true;
}

Coord Robot::directionToSink() {

	Coord dirToSink;
	if ( division == GOLD ) {
		//gold is on the left
		if (directionYToSink()) {
			dirToSink.y = 0;
			dirToSink.x =sgm(1 -pos.x);
		} else {
			dirToSink.y = sgm( mine->size.y/2 - pos.y);
			dirToSink.x =sgm(1 -pos.x);
		}
	} else {
		//waste on the right
		if (directionYToSink()) {
			dirToSink.y = 0;
			dirToSink.x =sgm(1 -pos.x);
		} else {
			dirToSink.y = sgm( mine->size.y/2 - pos.y);
			dirToSink.x =sgm(1 - pos.x);
		}
	}

	return dirToSink;
}

void Robot::setIndex(int _index ) { 
	index = _index; 
	mine->grid[pos.x][pos.y].index = _index; 
}
