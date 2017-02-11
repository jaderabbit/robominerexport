#include "DesertAnt.h"

#include <iostream>

DesertAnt::DesertAnt(void)
{
}


DesertAnt::DesertAnt(Robot* _robot) : RobotState(_robot)
{
}




DesertAnt::~DesertAnt(void)
{
}

void DesertAnt::doStep() {
	switch (robot->state) {
		case EXPLORING: exploring(); break;
		case HOMING: robot->homingStep(); break;
		case BEACON_HOMING: robot->beaconHomingStep(); break;
		case LOCATING: robot->locateStep(); break;
		case LOCAL_CLUSTER_SEARCH: robot->localClusterSearchMovement(); break;
		case LOADING: robot->loadStep(); break;
		case UNLOADING: robot->unloadStep(); break;
		case CHOOSE_ACTIVITY: robot->chooseActivity(); break;
		default: cout << "Erroneous state for Desert Ant" << endl; break;
	}
}

int DesertAnt::nextState(int currentState, bool located, bool memory, bool loaded) {
	if ( currentState == EXPLORING && located == true && memory == true && loaded == true ) {
		return HOMING;
	} else if ( currentState == HOMING && robot->isHome() && loaded == true ) {
		return UNLOADING;
	} else if ( currentState == UNLOADING && located == true && memory == true ) {
		return LOCATING;
	} else if ( currentState == LOCATING && located == true  ) {
		return HOMING;
	} else if ( currentState == LOCATING && located == false ) {
		return LOCAL_CLUSTER_SEARCH;
	} else if (  currentState == LOCAL_CLUSTER_SEARCH && located == false ) {
		return EXPLORING;
	} else if ( currentState == LOCAL_CLUSTER_SEARCH && located == true ) {
		return HOMING;
	} 
	return -1;
}

void DesertAnt::exploring() {
	if (robot->state_counter == 0 ) {
		robot->oldSinkPos.x = robot->pos.x;
		robot->oldSinkPos.y = robot->pos.y;
	}

	robot->state_counter++;

	//walk
	robot->randomWalkStep();

	if (robot->findItem()) {
		robot->density = robot->calculateDensity();
		
		//Set the memory;
		robot->clusterLocation = robot->previous_item_pos;
		robot->foraged = true;
		robot->typeForaged = robot->division;

		//State details
		robot->state_counter = 0;
		robot->activity_counter++;

		robot->state = nextState(robot->state,robot->foraged,true,true);
	} 
}
