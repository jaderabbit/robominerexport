#include "BasicForagingState.h"
#include "Robot.h"
#include "Mine.h"
#include <iostream>

BasicForagingState::BasicForagingState(Robot* _robot) : RobotState(_robot)
{
}

BasicForagingState::BasicForagingState(Robot* _robot, int _division, int max_search, int max_path) : RobotState(_robot), division(_division), MAX_PATH_DEVIATION(max_path), MAX_SEARCH_RANGE(max_search) {
	setMinorState(EXPLORING);
	major_state_counter = 0;
}

BasicForagingState::BasicForagingState(Robot* _robot, int _division) : RobotState(_robot), division(_division) {
	setMinorState(EXPLORING);
}

BasicForagingState::BasicForagingState(void)
{
	setMinorState(EXPLORING);
}

BasicForagingState::~BasicForagingState(void)
{
}

//Do Step
void BasicForagingState::doStep() {
	major_state_counter++;

	switch (state) {
		case HOMING: robot->setState(HOMING); robot->homingStep(); state = robot->getState(); break;
		case BEACON_HOMING: robot->setState(BEACON_HOMING); robot->beaconHomingStep(); state = robot->getState(); break;
		case LOCAL_CLUSTER_SEARCH: localClusterSearch(); break;
		case LOADING: load(); break;
		case UNLOADING: unload(); break;
		case EXPLORING: explore(); break;
		case CHOOSE_ACTIVITY: robot->chooseActivity(); break;
		default: cout << "Erroneous state for Foraging" << endl; break;
	}
}

void BasicForagingState::explore() {

	if (isFirstTime()) {
		robot->oldSinkPos.x = robot->pos.x;
		robot->oldSinkPos.y = robot->pos.y;
	}

	minor_state_counter++;

	//walk
	randomWalkStep();

	//look for item and pick up
	if (findItem()) {
		setMinorState(LOADING);
		major_state_counter++;
	} 

}

//Random wander
void BasicForagingState::randomWalkStep() {
	if ( robot->path_count < robot->path_length && robot->validMove() ) {
		makeMove();
		robot->path_count++;
	} else {
		if ( robot->path_count < robot->path_length ) {
			robot->chooseMaxPathLength();
		}
		
		robot->chooseDirection();

		/*if (t.randomOpen() < 0.5)
			robot->chooseDirection();
		else 
			chooseOppositeDirection();*/

		robot->path_count = 0;
	}
}

void BasicForagingState::makeMove() {
	if (robot->validMove()){
		robot->mine->grid[robot->pos.x][robot->pos.y].type = EMPTY; //prev pos to empty 
		robot->mine->grid[robot->pos.x + robot->dir.x][robot->pos.y + robot->dir.y].type = ROBOT; //current pos to robot
		robot->mine->grid[robot->pos.x + robot->dir.x][robot->pos.y + robot->dir.y].index = robot->mine->grid[robot->pos.x][robot->pos.y].index; //set the index of mine position to that of current robot
		
		//Move robot in direction
		robot->pos.x += robot->dir.x; 
		robot->pos.y += robot->dir.y; 


		//Basic Forage behaves like the explore
		robot->destination.x += robot->dir.x;
		robot->destination.y += robot->dir.y;

		robot->calculateDistanceFromSink();

		robot->moved = true;
	}
}

bool BasicForagingState::findItem() {
	for (int i=-1; i <= 1; i++) {
		for (int j=-1; j <= 1; j++) {
			if ( robot->validPos(robot->pos.x+i,robot->pos.y+j) && robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == robot->division) {
				robot->load_type = robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type;
				return true;
			}
			//if can go for any type
			else if ( division > WASTE && division > GOLD ) {
				if ( robot->validPos(robot->pos.x+i,robot->pos.y+j) && 
					( robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == GOLD || robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == WASTE)) {
						robot->load_type = robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type;
						return true;
				}
			}
		}
	}
	return false;
}

bool BasicForagingState::loadItem() {
	for (int i=-1; i <= 1; i++) {
		for (int j=-1; j <= 1; j++) {
			if ( robot->validPos(robot->pos.x+i,robot->pos.y+j) && robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == robot->division) {
				robot->load_type = robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type;
				robot->loaded = true;
				robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type = EMPTY;
				return true;
			}
			//if can go for any type
			else if ( division > WASTE && division > GOLD ) {
				if ( robot->validPos(robot->pos.x+i,robot->pos.y+j) && 
					( robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == GOLD || robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == WASTE)) {
						robot->load_type = robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type;
						robot->loaded = true;
						robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type = EMPTY;
						return true;
				}
			}
		}
	}
	return false;
}

void BasicForagingState::load() {
	//try find item
	if (loadItem()) {
		//do loading
		setMinorState(HOMING);

		//update cluster location
		robot->clusterLocation.x = robot->pos.x;
		robot->clusterLocation.y = robot->pos.y;

		robot->foraged = true;
		robot->typeForaged = division;
	} else {
		//do loading
		setMinorState(LOCAL_CLUSTER_SEARCH);
	}
}

void BasicForagingState::localClusterSearch() {
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

bool BasicForagingState::findItem( int searchRange ) {
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
			//if can go for any type
			else if ( division > WASTE && division > GOLD ) {
				if ( robot->validPos(robot->pos.x+i,robot->pos.y+j) && 
					( robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == GOLD || robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == WASTE)) {
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
	}
	return false;
}

void BasicForagingState::beaconHoming() {
	bool success = false;
	int rep_count = 0;

	//TODO: Improve how Robot gets the direction home.
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

void BasicForagingState::homing() {
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

void BasicForagingState::unload() {
	if (robot->loaded) {
		robot->loaded = false;
		robot->mine->sink_items[robot->load_type-1]++;

		//Set state
		 setMinorState(EXPLORING);
	}
	else {
		setMinorState(EXPLORING);
	}
}
