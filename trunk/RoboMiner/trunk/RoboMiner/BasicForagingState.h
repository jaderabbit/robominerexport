#pragma once
#include "robotstate.h"

// EXPLORE, LOADING, HOMING, UNLOADING

class BasicForagingState :
	public RobotState
{
public:

	BasicForagingState(void);
	BasicForagingState(Robot* _robot);
	virtual ~BasicForagingState(void);

	//Do Step
	virtual void doStep() =0;
};

