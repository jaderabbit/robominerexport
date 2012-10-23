#pragma once
#include "experiment.h"
class BasicForageGoldOnly :
	public Experiment
{
public:
	BasicForageGoldOnly(void);
	BasicForageGoldOnly(EXPERIMENT_DESC _desc);
	virtual ~BasicForageGoldOnly(void);

	virtual int initialize();
	virtual int run();
	virtual int runStep();
	virtual int cleanup();

	//Initialize
	void initializeGrid();
	void initializeObjects();
	void initializeRobots();
	void initializeSink();

	//Chose robot position
	Coord randomRobotPosition();

	const static int SINK_BOUNDARY = 5;
	static const int MAX_WANDER_STEPS = 50;

	int cnt;
};

