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
	//Increment time counter
	time++;
	num_robots = robots.size();
	//For each robot
	for (int i=0; i < robots.size(); i++) {
		//If the previous state of the robot is not equal to the current state then
		if ( getState(robots[i].getState()) == state ) {
			totalTimeSpentInState++;
		}
		if ( robots[i].foraged ) {
			totalItemsForaged++;
		}
	}

}

void AverageTimeInState::finalize() {
	finalStatistic = (totalItemsForaged <= 0) ? 0 : (totalItemsForaged > 0) ?  totalTimeSpentInState/(1.0*totalItemsForaged)/(num_robots*1.0) :  0;
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
	string s = "AverageTimeInState";
	s += getStateName(state);
	return s;
}


bool AverageTimeInState::isNext() {
	return ( timer > time ) ? false : true;
}

string AverageTimeInState::getNext() {
	stringstream s1;
	if (timer < time ) {
		timer++;
		return "";
	} else {		
		s1.clear();
		s1 << measurement[0];
		timer++;
		return s1.str();
	}
}


double AverageTimeInState::getNextValue() {
	//Return ratio at end
	return measurement[0];
}

double AverageTimeInState::getFinalValue() {
	//Return ratio at end
	return measurement[0];
}

