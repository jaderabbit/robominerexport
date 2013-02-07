#pragma once

#include "Tools.h"
#include "experiment.h"

class BeeForage :
	public Experiment
{
public:
	BeeForage(void);
	BeeForage( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc  );
	virtual ~BeeForage(void);

	//Main methods
	virtual int run();
	virtual int runStep();

	void initializePerformanceMeasures();
	virtual void initializeRobots();
	Coord randomRobotPosition();
};

