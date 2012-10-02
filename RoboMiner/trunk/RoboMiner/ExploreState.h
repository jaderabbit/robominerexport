#pragma once

#include "robotstate.h"
class ExploreState :
	public RobotState
{
public:
	ExploreState(void);
	ExploreState(Robot* _robot);
	ExploreState(Robot* _robot, int _MAX_RECRUITMENT_REPS, int _RADIUS_SIZE);
	virtual ~ExploreState(void);

	//Do Step
	virtual void doStep();
	void explore();

	//Explore and Recruit
	void exploreStep();
	void recruitStep();

	//Homing
	void homingStep();
	void beaconHomingStep();

	//Random wander
	void randomWalkStep();

	//Locating Items and Robots
	bool findItem();
	vector<int> searchSink();
	vector<int> radiusSearchSink();

	//Obstacle Avoidance
	void avoidObstacle();

	//Movement
	void makeMove();

	//State methods
	void setMinorState(int _state);
	bool isFirstTime();

protected:
	 int MAX_RECRUITMENT_REPS;
	 int RADIUS_SIZE;
};

