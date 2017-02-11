#pragma once
#include "robotstate.h"

// EXPLORE, LOADING, HOMING, UNLOADING

class BasicForagingState :
	public RobotState
{
public:
	//Constructors
	BasicForagingState(void);
	BasicForagingState(Robot* _robot);
	BasicForagingState(Robot* _robot, int _division);
	BasicForagingState(Robot* _robot, int _division, int max_search, int max_path);
	virtual ~BasicForagingState(void);

	//Do Step
	virtual void doStep();

	//states
	void explore();
	void load();
	void localClusterSearch();
	void homing();
	void beaconHoming();
	void unload();

	//Random walk
	void randomWalkStep();
	bool loadItem();
	bool findItem(); //only boolean
	bool findItem( int searchRange );
	void makeMove();

	//Getters and setters
	void setDivision( int _division ) { division = _division; }

private:
	int division;
	int MAX_SEARCH_RANGE;
	int MAX_PATH_DEVIATION;
};

