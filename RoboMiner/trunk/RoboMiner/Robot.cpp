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
#include "DesertAnt.h"

Robot::Robot(void)
{
	one_stuck_mother_fucker = 0;

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

	lambda = 0.5;
	path_count = 0;

	one_stuck_mother_fucker = 0;

	desirability_threshold =0;
	desirability_total=0;
	num_desirabilities=0;

	recruitment_reps = MAX_RECRUITMENT_REPS;
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
		case (DESERTANT) : robotState->doStep(); break;
		default: cout << "Erroneous ACTIVITY" << endl; break;
	}

	if ( moved == false && state == LOCATING  ) {
		one_stuck_mother_fucker++;
	} else {
		one_stuck_mother_fucker = 0;
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
		mine->grid[pos.x + dir.x][pos.y + dir.y].index = mine->grid[pos.x][pos.y].index; //set the index of mine position to that of current robot
		
		//Move robot in direction
		pos.x += dir.x; 
		pos.y += dir.y; 

		if (activity == EXPLORE || (activity == DESERTANT && state == EXPLORING)) {
			//TODO: Possibly irrelevant line of code. Optimize
			destination.x += dir.x;
			destination.y += dir.y;
		} else if (activity == FORAGE || (activity == DESERTANT && state == LOCATING) ) {
			destination.x -= dir.x;
			destination.y -= dir.y;
		} else if (activity == BASICFORAGE ) {
			//Basic Forage behaves like the explore
			destination.x += dir.x;
			destination.y += dir.y;
		}

		//Calculate distance from sink. 
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

double Robot::calculatePosDistanceFromSink(Coord new_pos) {

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
	while ( curr < 5 && i <= 8/2 ) {
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

bool Robot::validMove( int dx, int dy ) {
	return validPos( pos.x + dx, pos.y + dy );
}

bool Robot::validMove( Coord p ) {
	return validPos( pos.x + p.x, pos.y + p.y );
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
	if ( pos.x < mine->SINK_BOUNDARY )
		delta = alpha; //Do not combine if within sink boundary as then there are no obstacles
		//TODO: As soon as there exists no boundaries at any point, not only within the sink boundary, we have a problem.
	else
		delta = lambda*alpha + (1-lambda)*beta;

	return delta;
}

void  Robot::chooseForagerDirection() {
	//Determine if stuck and empty vicinity. 
	adaptLambda();

	//choose initial direction
	dir = directionToSink();

	//calculate FoV
	int num = calculateFoV();

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

	if (isStuck()) {
		dir = FoV[t.random(0,7)];
	}
}

bool Robot::isStuck() {
	//Stuck 
	if ( one_stuck_mother_fucker > 30 ) {
		return true;
	}

	//push back previous direction and pop off oldest one
	stuck_window.push_back(dir);

	//Pop off oldest one if queue greater than the stuck window size. 
	if (  stuck_window.size() >= STUCK_WINDOW_SIZE ) {
		stuck_window.pop_front();
	} else {
		return false;
	}

	//add all coords
	Coord total; total.x = 0; total.y = 0;
	for (int i=0; i < stuck_window.size(); i++) {
		//add the coordinates
		total.x += stuck_window[i].x;
		total.y += stuck_window[i].y;
	}

	//If all directions add up to  basically nothing then return true as robot has not really moved;
	if ( (total.x >= -1 && total.x <= 1 ) && (total.y >= -1 && total.y <= 1)  ) {
		return true;
	}

	//Return false
	return false;
}

bool Robot::isEmptyVicinity() {
	for (int i=0; i < 8; i++ ) {
		if ( !isEmpty(dir_circle[i].x, dir_circle[i].y ) ) {
			return false;
		}
	}

	return true;
}

int Robot::robotInVicinity() {
	int k=0;
	for (int i=0; i < 8; i++ ) {
		if ( !isEmpty(dir_circle[i].x, dir_circle[i].y ) ) {
			k++;
		}
	}

	return k;
}

void Robot::adaptLambda() {
	bool stuck = isStuck();
	bool empty =  isEmptyVicinity();

	double lambdaDelta = 0.1;

	if ( stuck && empty ) {
		//decrement clarity thus increment lambda
		if (lambda <= 1 - lambdaDelta ) 
			lambda += lambdaDelta;
	} else if ( stuck && !empty ) {
		//increment clarity usage
		if (lambda -lambdaDelta >=  -0.4 ) 
			lambda -= lambdaDelta;
	} else { //if not stuck, reset
		if ( lambda < 0.5 ) {
			lambda += lambdaDelta;
		} else if (lambda > 0.5) {
			lambda -= lambdaDelta;
		}
		
	}
}

void  Robot::chooseForagerLocatingDirection() {
	//Determine if stuck and empty vicinity. 
	adaptLambda();

	//choose initial direction to Cluster
	dir = directionToItem();

	//calculate FoV
	int num = calculateFoV();

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

	if (isStuck()) {
		dir = FoV[t.random(0,7)];
	}
}

Coord  Robot::directionToItem() {
	Coord dirToItem;

	//Add gaussian noise to clusterLocation

	//TODO: Determine whether a temporary should be used or the most current cluster location. 
	//TODO: Determine whether gaussian noise should be added always.
	if (state_counter > MAX_STATE_COUNTER ) {
		clusterLocation.x = t.gaussianDistributionDiscrete(clusterLocation.x, MAX_PATH_DEVIATION);
		clusterLocation.y = t.gaussianDistributionDiscrete(clusterLocation.y, MAX_PATH_DEVIATION);
		state_counter = MAX_STATE_COUNTER - state_counter;
	}

	//Calculate Direction to Items
	dirToItem.x = sgm(clusterLocation.x - pos.x);
	dirToItem.y = sgm(clusterLocation.y - pos.y);

	return dirToItem;

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
	} else if (state_counter == 0 && activity == FORAGE || activity == DESERTANT) {
		//set the location of the cluster
		clusterLocation.x =  pos.x;
		clusterLocation.y =  pos.y;

		destination.x = oldSinkPos.x;
		destination.y = oldSinkPos.y;
	} else if (state_counter == 0 && activity == BASICFORAGE) {
		//set the location of the cluster
		clusterLocation.x =  pos.x;
		clusterLocation.y =  pos.y;

		//set home as the destination
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
			else if (activity == FORAGE || activity == DESERTANT) {
				state = UNLOADING;
			} else if (activity == BASICFORAGE) {
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
		} else if (activity == FORAGE || activity == DESERTANT) {
			state = UNLOADING;
		} else if ( activity == BASICFORAGE) {
			state = UNLOADING;
		}
	}

	//New method
	chooseForagerDirection();

	//make move
	makeMove();

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

double Robot::calculateLocationDesirability( double distance, double density ) {
	//use maximum distance from sink - make sure this is updated. 

	//use distance of food source from sink

	//Use density.

	//for now balance them equally
	int desirability_balance = 0.5;
	//NOTE: Density if inverted as we are using minimum
	return desirability_balance*distance + (1-desirability_balance)*(1-density);
	//return density;
}