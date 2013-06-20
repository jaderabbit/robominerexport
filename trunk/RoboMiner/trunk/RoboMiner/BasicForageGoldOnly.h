#pragma once
#include "Experiment.h"
class BasicForageGoldOnly :
	public Experiment
{
public:
	BasicForageGoldOnly(void);
	BasicForageGoldOnly(EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc );
	virtual ~BasicForageGoldOnly(void);

	virtual int run();
	virtual int runStep();
	virtual int cleanup();

	//Initialize
	virtual void initializeRobots();

	//Chose robot position
	Coord randomRobotPosition();

	const static int SINK_BOUNDARY = 5;
	static const int MAX_WANDER_STEPS = 50;
};

