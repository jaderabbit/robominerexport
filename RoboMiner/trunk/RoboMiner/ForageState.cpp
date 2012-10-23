#include "ForageState.h"
#include <iostream>
#include "Mine.h"

ForageState::ForageState(void)
{
}

ForageState::ForageState( Robot* _robot) : RobotState(_robot)
{
	setMinorState(WAITING);
}


ForageState::~ForageState(void)
{
}

void ForageState::doStep() 
{
	forage();
}

void ForageState::forage() {

	switch (state) {
		case HOMING: homingStep(); break;
		case BEACON_HOMING: beaconHomingStep(); break;
		case WAITING: waitStep(); break;
		case LOCATING: locateStep(); break;
		case LOCAL_CLUSTER_SEARCH: localClusterSearchMovement(); break;
		case LOADING: loadStep(); break;
		case UNLOADING: unloadStep(); break;
		case CHOOSE_ACTIVITY: robot->chooseActivity(); break;
		default: cout << "Erroneous state for Foraging" << endl; break;
	}
}

void ForageState::waitStep() {
	if (isFirstTime()) {
		robot->oldSinkPos.x = robot->pos.x;
		robot->oldSinkPos.y = robot->pos.y;
	}
	minor_state_counter++;
}

void ForageState::locateStep() {
	//follow cluster location. 
	do { 
		robot->dir.x = (robot->t.randomOpen() < 0.5) ? robot->sgm(robot->destination.x) : 0;
		robot->dir.y = (robot->t.randomOpen() < 0.5) ? robot->sgm(robot->destination.y) : 0;
	} while ( robot->dir.x == 0 && robot->dir.y == 0 );

	//assert(dir.x != 0 && dir.y != 0 );

	minor_state_counter++;
	int rep_count =0;
	bool madeMove = false;
	do { 
		if (robot->validMove()) {
			robot->makeMove();
			madeMove =true;
			if ( robot->destination.x ==0 && robot->destination.y ==0 ) {
				setMinorState(LOADING);
			}
		} else {
			if (seeItem()) {
				setMinorState(LOADING);
				madeMove =true;
			} else if ( walkingIntoAWall() ) {
				setMinorState(LOADING);
				madeMove =true;
			} else {
				robot->avoidObstacle();
			}
		}
		rep_count++;
	} while ( rep_count < 8 && madeMove==false);

}

bool ForageState::seeItem() {
	for (int i=-1; i <= 1; i++) {
		for (int j=-1; j <= 1; j++) {
			if ( robot->validPos(robot->pos.x+i,robot->pos.y+j) && robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == robot->division) {
				return true;
			}
		}
	}
	return false;
}

void ForageState::loadStep() {
	//try find item
	if (robot->findItem()) {
		//do loading
		setMinorState(HOMING);

		//update cluster location
		robot->clusterLocation.x = robot->pos.x;
		robot->clusterLocation.y = robot->pos.y;
	} else {
		//do loading
		setMinorState(LOCAL_CLUSTER_SEARCH);
	}
}

void ForageState::unloadStep() {
	if (robot->loaded) {
		robot->loaded = false;
		robot->mine->sink_items[robot->load_type-1]++;

		//Old sink position
		robot->oldSinkPos.x = robot->pos.x;
		robot->oldSinkPos.y = robot->pos.y;

		Coord location;
		
		//Triangulate position - final direction = broadcasted location - (position of ForageState - position of recruiter)
		location.x = robot->recruiterClusterLocation.x - ( robot->pos.x - robot->recruiterOriginalPos.x);
		location.y = robot->recruiterClusterLocation.y - ( robot->pos.y - robot->recruiterOriginalPos.y);

		//TODO: Improvement. Currently, add gaussian noise to deviate from absolute position. Dependant on size of devergence of noise
		location.x =  robot->t.gaussianDistributionDiscrete( location.y,MAX_PATH_DEVIATION);

		//Cluster
		 robot->clusterLocation.x = location.x;
		 robot->clusterLocation.y = location.y;

		//Set home vector
		 robot->homeVector.x = -location.x;
		 robot->homeVector.y = -location.y;

		//set destination
		 robot->destination.x = location.x;
		 robot->destination.y = location.y;

		//Set state
		 setMinorState(LOCATING);

	}
	else {
		setMinorState(WAITING);
	}
}

void ForageState::addRecruiterMessage( Coord location, Coord recruiterPos, int type ) {
	
	//Recruiter Message - use these later on
	robot->recruiterOriginalPos.x = recruiterPos.x;
	robot->recruiterOriginalPos.y = recruiterPos.y;
	robot->recruiterClusterLocation.x = location.x;
	robot->recruiterClusterLocation.y = location.y;

	//Triangulate position - final direction = broadcasted location - (position of ForageState - position of recruiter)
	location.x = location.x - ( robot->pos.x - recruiterPos.x);
	location.y = location.y - ( robot->pos.y - recruiterPos.y);

	//Add gaussian noise to deviate a bit - MAY OR MAY NOT WORK! EEK!
	location.x = robot->t.gaussianDistributionDiscrete(location.x,MAX_PATH_DEVIATION);
	location.y = robot->t.gaussianDistributionDiscrete(location.y,MAX_PATH_DEVIATION);

	//Cluster
	robot->clusterLocation.x = location.x;
	robot->clusterLocation.y = location.y;

	//Set home vector
	robot->homeVector.x = -location.x;
	robot->homeVector.y = -location.y;

	//set destination
	robot->destination.x = location.x;
	robot->destination.y = location.y;

	//Set state
	setMinorState(LOCATING);

	//test to see how often recruited
	major_state_counter++;

	//load
	robot->division = type;

}

void ForageState::localClusterSearchMovement() {
	if (minor_state_counter <= MAX_SEARCH_RANGE) {
		//increment repititions
		minor_state_counter++;

		//attempt to find item
		bool found = findItem(minor_state_counter);

		//if item has been found and loaded
		if (found && robot->loaded) {
			setMinorState(HOMING);

			//update cluster location - more accurate and closer
			robot->clusterLocation.x = robot->pos.x;
			robot->clusterLocation.y = robot->pos.y;
		//if item has been located but not loaded
		} else if (found) {
			robot->makeMove();
		}

	} else {
		//else cluster is depleted and just go home unloaded 
		setMinorState(HOMING);
	}
}

bool ForageState::findItem( int searchRange ) {
	for (int i=-searchRange; i <= searchRange; i++) {
		for (int j=-searchRange; j <= searchRange; j++) {
			if ( robot->validPos(robot->pos.x+i,robot->pos.y+j) && robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == robot->division) {
				//if directly next to it then load
				if ( i == robot->sgm(i) && j == robot->sgm(j) ) {
					robot->load_type = robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type;
					robot->loaded = true;
					robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type = EMPTY;
					return true;
				} else {
					//else set direction to move towards it
					robot->dir.x = robot->sgm(i); robot->dir.y = robot->sgm(j);
				}
			}
		}
	}
	return false;
}


void ForageState::beaconHomingStep() {
	bool success = false;
	int rep_count = 0;

	//get direction home - HACK - cuz wer
	robot->dir.x = robot->sgm(0 - robot->pos.x);
	if ( robot->load_type == GOLD ) {
		//gold is on the left
		if (  robot->pos.y >= robot->mine->size.y/2 ) {
			robot->dir.y = -1;
		} else {
			robot->dir.y = 0;
		}
	} else {
		if ( robot->pos.y <= robot->mine->size.y/2 ) {
			robot->dir.y = 1;
		} else {
			robot->dir.y = 0;
		}
	}

	do {
		if ( robot->isHome() ) { //this was wrong. 
			setMinorState(UNLOADING);
			success = true;
			break;
		}
		//move is valid
		if (robot->validMove() ) {
			//make move
			robot->makeMove();

			//successfully moved
			success = true;

		} else {
			//if something in the way avoid obstacle
			robot->avoidObstacle();
			rep_count++;
		}
	} while (!success && rep_count < 8); //rep count < 8 to try all sides

	//increment state counter
	minor_state_counter++;
}

void ForageState::homingStep() {
	if (isFirstTime()) {
		robot->destination.x = robot->homeVector.x;
		robot->destination.y = robot->homeVector.y;
	}

	//increment state counter
	minor_state_counter++;

	//need to use accumulated direction vector to guide homing instinct. 
	robot->dir.x = robot->sgm(robot->destination.x);
	robot->dir.y = robot->sgm(robot->destination.y);

	//check if valid move
	if (robot->validMove()) {
		//make the move
		robot->makeMove();

		//check if home
		if (robot->isHome() ) { //this clause makes sure it is home
			setMinorState(UNLOADING);
		}
	//if move is not valid - switch to beacon homing
	} else {
		setMinorState(BEACON_HOMING);
	}
}

bool ForageState::walkingIntoAWall(){
	if ( (robot->pos.x + robot->dir.x >= 0) && (robot->pos.x + robot->dir.x < robot->mine->size.x)  && (robot->pos.y + robot->dir.y < robot->mine->size.y) && (robot->pos.y + robot->dir.y >= 0)) {
		return false;
	}
	return true;
}
