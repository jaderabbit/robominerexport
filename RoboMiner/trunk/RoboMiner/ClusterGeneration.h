#pragma once
#include "experiment.h"
class ClusterGeneration :
	public Experiment
{
public:
	ClusterGeneration(void);
	~ClusterGeneration(void);
	ClusterGeneration(Tools &t);
	ClusterGeneration( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc  );
	virtual int initialize();

	
	//Main methods
	virtual int run();
	virtual int runStep();

	virtual void initializeRobots();
	virtual string getAlgorithmName() { return "ClusterGeneration"; }
	Coord randomRobotPosition();
};

