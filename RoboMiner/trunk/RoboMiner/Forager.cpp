#include "Robot.h"
#include "Mine.h"
#include <iostream>
#include <assert.h>
#include "RobotState.h"
#include "PerformanceBed.h"

using namespace std;

void Robot::forage() {

	switch (state) {
		case HOMING: homingStep(); break;
		case BEACON_HOMING: beaconHomingStep(); break;
		case WAITING: waitStep(); break;
		case LOCATING: locateStep(); break;
		case LOCAL_CLUSTER_SEARCH: localClusterSearchMovement(); break;
		case LOADING: loadStep(); break;
		case UNLOADING: unloadStep(); break;
		case CHOOSE_ACTIVITY: chooseActivity(); break;
		default: cout << "Erroneous state for Foraging" << endl; break;
	}
}

void Robot::waitStep() {
	if (state_counter == 0) {
		oldSinkPos.x = pos.x;
		oldSinkPos.y = pos.y;
	}
	state_counter++;
}

void Robot::locateStep() {
	//TODO: Add obstacle avoidance to locating
	//set destination
	if (state_counter == 0 && activity == FORAGE ) {	
		//set home as the destination
		destination.x = clusterLocation.x;
		destination.y = clusterLocation.y;
	}
	
	//follow cluster location. 
	/*do { 
		double r1 = t.randomOpen(), r2 = t.randomOpen();
		dir.x = (t.randomOpen() < 0.5) ? sgm(clusterLocation.x - pos.x) : 0;
		dir.y = (t.randomOpen() < 0.5) ? sgm(clusterLocation.y - pos.y) : 0;
	} while ( dir.x == 0 && dir.y == 0 && ((clusterLocation.x - pos.x) != 0 &&  (clusterLocation.y - pos.y != 0)));

	//assert(dir.x != 0 && dir.y != 0 );


	if (state_counter > MAX_STATE_COUNTER ) {
		clusterLocation.x = t.gaussianDistributionDiscrete(clusterLocation.x, MAX_PATH_DEVIATION);
		clusterLocation.y = t.gaussianDistributionDiscrete(clusterLocation.y, MAX_PATH_DEVIATION);
	} */

	//Choose the locating direction of the forager.
	chooseForagerLocatingDirection();

	//Increment state counter
	state_counter++;

	//Move making
	//TODO: Optimize following piece of logic which determines what action to take.

	//Make the move: Does validity check
	makeMove();

	if ( clusterLocation.x - pos.x ==0 && clusterLocation.y - pos.y ==0 ) {
		state_counter = 0;
		state = LOADING;
	} else if (seeItem()) {
		state = LOADING;
		state_counter = 0;
	} else if ( walkingIntoAWall() ) {
		state = LOADING;
		state_counter = 0;
	} 
}

bool Robot::seeItem() {
	for (int i=-1; i <= 1; i++) {
		for (int j=-1; j <= 1; j++) {
			if ( validPos(pos.x+i,pos.y+j) && mine->grid[pos.x +i][pos.y+j].type == division) {
				return true;
			}
		}
	}
	return false;
}

void Robot::loadStep() {
	//try find item
	if (findItem()) {
		//do loading
		state_counter = 0;
		state = HOMING;

		//update cluster location
		clusterLocation.x = pos.x;
		clusterLocation.y = pos.y;

		foraged = true;
		typeForaged = division;
	} else {
		//do loading
		state_counter = 0;
		state = LOCAL_CLUSTER_SEARCH;
	}
}

void Robot::unloadStep() {
	if (loaded) {
		//unload
		loaded = false;
		mine->sink_items[load_type-1]++;

		//Old sink position
		oldSinkPos.x = pos.x;
		oldSinkPos.y = pos.y;

		//destination is set back to the location of the cluster
		destination = clusterLocation;

		//Set state
		state = LOCATING;
		state_counter = 0;

	}
	else {
		state_counter = 0;
		state = WAITING;
		reset();
		//cout << "cluster depleted, returned home empty" << endl;
	}
}

void Robot::addRecruiterMessage( Coord location, Coord recruiterPos, int type ) {

	//Global Location of cluster. 
	clusterLocation = location;
	destination = location;

	//Set the destination to the cluster location
	clusterLocation.x = t.gaussianDistributionDiscrete(location.x,MAX_PATH_DEVIATION);
	clusterLocation.y = t.gaussianDistributionDiscrete(location.y,MAX_PATH_DEVIATION);
	destination = clusterLocation;

	//check if in bounds
	if (clusterLocation.x <= 0 ) clusterLocation.x = 1;
	if (clusterLocation.y >= mine->size.y ) clusterLocation.y = mine->size.y-1;

	//Use recruiter position to determine whether to take it or not

	//change state
	state = LOCATING;
	state_counter = 0;

	//robotState->setMinorState(LOCATING);
	//test to see how often recruited
	activity_counter++;

	//load
	division = type;

		/* //Move back to top
	//Recruiter Message - use these later on
	recruiterOriginalPos.x = recruiterPos.x;
	recruiterOriginalPos.y = recruiterPos.y;
	recruiterClusterLocation.x = location.x;
	recruiterClusterLocation.y = location.y;

	//Triangulate position - final direction = broadcasted location - (position of robot - position of recruiter)
	location.x = location.x - ( pos.x - recruiterPos.x);
	location.y = location.y - ( pos.y - recruiterPos.y);

	//Add gaussian noise to deviate a bit - MAY OR MAY NOT WORK! EEK!
	location.x = t.gaussianDistributionDiscrete(location.x,MAX_PATH_DEVIATION);
	location.y = t.gaussianDistributionDiscrete(location.y,MAX_PATH_DEVIATION);

	//Cluster
	clusterLocation.x = location.x;
	clusterLocation.y = location.y;

	//Set home vector
	homeVector.x = -location.x;
	homeVector.y = -location.y;

	//set destination
	destination.x = location.x;
	destination.y = location.y;*/

	//location



}

void Robot::localClusterSearchMovement() {
	if (state_counter <= MAX_SEARCH_RANGE) {
		//increment repititions
		state_counter++;

		//attempt to find item
		bool found = findItem(state_counter);

		//if item has been found and loaded
		if (found && loaded) {
			state = HOMING;
			state_counter = 0;

			//update cluster location - more accurate and closer
			clusterLocation.x = pos.x;
			clusterLocation.y = pos.y;

			foraged = true;
			typeForaged = load_type;

		//if item has been located but not loaded
		} else if (found) {
			makeMove();
			//state_counter--;
		}

	} else {
		//else cluster is depleted and just go home unloaded 
		state = HOMING;
		state_counter = 0;
	}
}

bool Robot::findItem( int searchRange ) {
	for (int i=-searchRange; i <= searchRange; i++) {
		for (int j=-searchRange; j <= searchRange; j++) {
			if ( validPos(pos.x+i,pos.y+j) && mine->grid[pos.x +i][pos.y+j].type == division) {
				//if directly next to it then load
				if ( i == sgm(i) && j == sgm(j) ) {
					load_type = mine->grid[pos.x +i][pos.y+j].type;
					loaded = true;
					mine->grid[pos.x +i][pos.y+j].type = EMPTY;
					return true;
				} else {
					//else set direction to move towards it
					dir.x = sgm(i); dir.y = sgm(j);
				}
			}
		}
	}
	return false;
}