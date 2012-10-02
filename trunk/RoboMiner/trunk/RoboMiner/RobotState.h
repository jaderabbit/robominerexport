#pragma once

#include "Robot.h"

class RobotState
{
public:
	RobotState(void);
	RobotState(Robot* _robot);
	virtual ~RobotState(void);

	//Do Step
	virtual void doStep() =0;
protected:
	Robot* robot;

	//state within state
	int state;

	//counts the number of steps that have occured in the state of a state
	int minor_state_counter;
	int major_state_counter;

};

