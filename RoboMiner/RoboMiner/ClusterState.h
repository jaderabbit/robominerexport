#pragma once
#include "robotstate.h"
class ClusterState :
	public RobotState
{
public:
	ClusterState(void);
	ClusterState(Robot* _robot);
	~ClusterState(void);

	virtual void doStep();

	void cluster();

	//CLUSTERING Methods
	void unloadedStep();
	void loadedStep();
	void calculateF();
	bool load();
	bool unload();

	//Helper CLUSTERING Methods
	void randomWalkStep();
	void lookAround();
	void chooseDirection();
	void chooseOppositeDirection();
	void chooseMaxPathLength();

	bool validPos(int x, int y);
};

