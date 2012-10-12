#include "RobotState.h"

RobotState::RobotState(Robot* _robot) : robot(_robot)
{
}

RobotState::RobotState(void)
{
}


RobotState::~RobotState(void)
{
}


void RobotState::setMinorState(int _state) {
	minor_state_counter = 0;
	state = _state;
	robot->state = _state;
}

bool RobotState::isFirstTime() { 
	return minor_state_counter == 0; 
}
