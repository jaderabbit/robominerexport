#pragma once
#include "robotstate.h"


class DesertAnt :
	public RobotState
{
public:
	DesertAnt(void);
	DesertAnt(Robot* _robot);
	virtual ~DesertAnt(void);

	int nextState(int currentState, bool located, bool memory, bool loaded);
	virtual void doStep();

	void exploring();

};

