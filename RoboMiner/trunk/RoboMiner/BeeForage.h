#pragma once

#include "Tools.h"
#include "experiment.h"

class ClusterForage :
	public Experiment
{
public:
	ClusterForage(void);
	ClusterForage( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc  );
	virtual ~ClusterForage(void);

	//Main methods
	virtual int run();
	virtual int runStep();
	virtual int runAllSamplesStep();
	void initializePerformanceMeasures();
	virtual void initializeRobots();
	Coord randomRobotPosition();
	virtual int getTotalIterations();
};

