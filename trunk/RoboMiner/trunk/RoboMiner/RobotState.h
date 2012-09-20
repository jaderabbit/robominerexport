#pragma once
class RobotState
{
public:
	RobotState(void);
	~RobotState(void);

	//Do Step
	virtual void doStep() =0;
};

