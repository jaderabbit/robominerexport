#include "ExploreState.h"
#include "Mine.h"
#include "ForageState.h"

#include <iostream>

using namespace std;

ExploreState::ExploreState(void)
{
	//set initial state 
	state = EXPLORING;
}

ExploreState::ExploreState(Robot* _robot) : RobotState(_robot)
{
	//Initial State
	setMinorState(EXPLORING);

	//Default values
	MAX_RECRUITMENT_REPS = 30;
	RADIUS_SIZE = 10;
}

ExploreState::ExploreState(Robot* _robot, int _MAX_RECRUITMENT_REPS, int _RADIUS_SIZE) : RobotState(_robot), MAX_RECRUITMENT_REPS(_MAX_RECRUITMENT_REPS), RADIUS_SIZE(_RADIUS_SIZE)
{
}

ExploreState::~ExploreState(void)
{
}

void ExploreState::doStep()
{
	major_state_counter++;
	explore();
}

void ExploreState::explore() {
	switch (state) {
		case HOMING: homingStep(); break;
		case BEACON_HOMING: beaconHomingStep(); break;
		case EXPLORING: exploreStep(); break;
		case RECRUITING: recruitStep(); break;
		case CHOOSE_ACTIVITY: robot->chooseActivity(); break;
		default: cout << "Erroneous State for Exploring" << endl; break;
	}
}

void ExploreState::avoidObstacle() {
	Coord option[2];
	if ( robot->dir.x == 0 || robot->dir.y == 0) {
		//option 1
		option[0].x = robot->dir.x + robot->dir.y;
		option[0].y = robot->dir.y + robot->dir.x;

		//option 2
		option[1].x = robot->dir.x - robot->dir.y;
		option[1].y = robot->dir.y - robot->dir.x;
	} else {
		//option 1
		option[0].x = robot->dir.x;
		option[0].y = 0;

		//option 1
		option[1].x = 0;
		option[1].y = robot->dir.y;
	}

	//choose to go LEFT or RIGHT
	int d =  (robot->t.randomClosed() >= 0.5 ) ? 1 : 0;
	robot->dir = option[d];
}

void ExploreState::exploreStep() {

	if (isFirstTime()) {
		robot->oldSinkPos.x = robot->pos.x;
		robot->oldSinkPos.y = robot->pos.y;
	}

	minor_state_counter++;

	//walk
	randomWalkStep();

	//look for item and pick up
	if (findItem()) {
		setMinorState(HOMING);
		major_state_counter++;
	} 

}

void ExploreState::randomWalkStep() {
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

bool ExploreState::findItem() {
	for (int i=-1; i <= 1; i++) {
		for (int j=-1; j <= 1; j++) {
			if ( robot->validPos(robot->pos.x+i,robot->pos.y+j) && robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == robot->division) {
				robot->load_type = robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type;
				return true;
			}
		}
	}
	return false;
}

void ExploreState::recruitStep() {
	if (minor_state_counter < MAX_RECRUITMENT_REPS ) {
		//search up and down the area by the sink for waiting robots
		vector<int> waitingRobots = searchSink();

		//send message to waiting robots
		for (int i=0; i < waitingRobots.size(); i++) {
			if ( robot->mine->robots[waitingRobots[i]].state == WAITING) {
				robot->mine->robots[waitingRobots[i]].addRecruiterMessage(robot->clusterLocation,robot->oldSinkPos,robot->division,robot->density);
			}
		}
		minor_state_counter++;	
	} else {
		setMinorState(EXPLORING);
		robot->reset();
	}
}

vector<int> ExploreState::searchSink() {
	vector<int> waiting;

	for (int i=0; i < robot->mine->size.y; i++) {
		if ( robot->mine->grid[1][i] == ROBOT ) {
			waiting.push_back(robot->mine->grid[1][i].index);
		}
	}

	return waiting;
}

vector<int> ExploreState::radiusSearchSink() {
	vector<int> waiting;

	for (int i=0; i < RADIUS_SIZE; i++) {
		if ( robot->pos.y-i > 0 && robot->mine->grid[1][robot->pos.y - i] == ROBOT ) {
			waiting.push_back(robot->mine->grid[1][robot->pos.y - i].index);
		}
		if ( robot->pos.y +i < robot->mine->grid.size() && robot->mine->grid[1][robot->pos.y + i] == ROBOT ) {
			waiting.push_back(robot->mine->grid[1][robot->pos.y + i].index);
		}
	}

	return waiting;
}

void ExploreState::makeMove() {
	robot->mine->grid[robot->pos.x][robot->pos.y].type = EMPTY; //prev pos to empty 
	robot->mine->grid[robot->pos.x + robot->dir.x][robot->pos.y + robot->dir.y].type = ROBOT; //current pos to robot
	robot->mine->grid[robot->pos.x + robot->dir.x][robot->pos.y + robot->dir.y].index = robot->mine->grid[robot->pos.x][robot->pos.y].index;
	robot->pos.x += robot->dir.x;
	robot->pos.y += robot->dir.y;

	robot->destination.x += robot->dir.x;
	robot->destination.y += robot->dir.y;
}

void ExploreState::homingStep() {
	//set destination
	if (isFirstTime()) {	
		//set the location of the cluster
		robot->clusterLocation.x =  robot->destination.x;
		robot->clusterLocation.y =  robot->destination.y;

		//set home vector
		robot->homeVector.x = -robot->destination.x;
		robot->homeVector.y = -robot->destination.y;

		//set home as the destination
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
		makeMove();

		if ( robot->isHome() ) { 
			setMinorState(RECRUITING);
		}

	//if move is not valid - switch to beacon homing
	} else {
		setMinorState(BEACON_HOMING);
	}
}

void ExploreState::beaconHomingStep() {
	bool success = false;
	int rep_count = 0;

	//get direction home
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
		//This hack is a LIE. this is a weird hack to check if the robot is home - need to also ensure that robot is in ACTIVITY = RECRUITING
		if ( robot->isHome() ) { //this was wrong. 
			setMinorState(RECRUITING);
			success = true;
			break;
		}
		//move is valid
		if (robot->validMove() ) {
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
	minor_state_counter++;
}

