#include "AverageTimeInState.h"



AverageTimeInState::AverageTimeInState(void) : PerformanceMeasure()
{
}


AverageTimeInState::~AverageTimeInState(void)
{
}

AverageTimeInState::AverageTimeInState( int _state) : PerformanceMeasure(), state(_state)
{
	time = 0;
}

void AverageTimeInState::takeMeasure( vector<Robot>& robots) 
{
	//Initialize arrays storing measures
	if ( time == 0 )
	{
		for (int i=0; i < robots.size(); i++) 
		{
			timeSpentInStatePerRobot.push_back(0);
			itemsForagedPerRobot.push_back(0);
		}
	}

	//Increment time counter
	time++;

	//For each robot
	for (int i=0; i < robots.size(); i++) {
		//If the previous state of the robot is not equal to the current state then
		if ( getState(robots[i].getState()) == state ) {
			timeSpentInStatePerRobot[i]++;
		}
		if ( robots[i].foraged ) {
			itemsForagedPerRobot[i]++;
		}
	}
}

void AverageTimeInState::finalize() {
	double totalTimeInState = 0;
	double totalItemsForaged = 0;
	for (int i=0; i < timeSpentInStatePerRobot.size(); i++) {
		totalTimeInState+= timeSpentInStatePerRobot[i];
		totalItemsForaged+= itemsForagedPerRobot[i];
	}

	finalStatistic = totalTimeInState/totalItemsForaged/30.0;
	measurement.push_back(finalStatistic);
}

int AverageTimeInState::getState(int _state) {
		switch (_state) {
		//Locating
		case LOCATING: return PM_FORAGE;
		case LOCAL_CLUSTER_SEARCH: return PM_FORAGE;
		case LOADING: return PM_FORAGE;

		//Exploring
		case EXPLORING: return PM_EXPLORE;

		//Returning
		case HOMING : return PM_RETURN;
		case BEACON_HOMING : return PM_RETURN;

		//Waiting
		case WAITING : return PM_WAIT;

		//Recruiting
		case RECRUITING : return PM_RECRUIT;

		//default
		default: return PM_OTHER;
	}
}

string AverageTimeInState::getStateName( int _state) {
	switch (_state) {
		case PM_EXPLORE: return "EXPLORE"; 
		case PM_FORAGE: return "FORAGE"; 
		case PM_RECRUIT: return "RECRUIT";
		case PM_RETURN: return "RETURN";
		case PM_WAIT: return "WAIT"; 
		default: return "OTHER";
	}
}

string AverageTimeInState::getName() { 
	string s = "ItemsForagedOverTime";
	s += getStateName(getState(state));
	return s;
}


bool AverageTimeInState::isNext() {
	return ( timer >= time ) ? false : true;
}

string AverageTimeInState::getNext() {
	if (timer < time - 1 ) {
		timer++;
		return "";
	} else {		
		s.clear();
		s << measurement[0];
		timer++;
		return s.str();
	}
}
