#pragma once

#include "Tools.h"
#include "experiment.h"

class BeeForage :
	public Experiment
{
public:
	BeeForage(void);
	BeeForage(Tools &_t);
	BeeForage( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc  );
	virtual ~BeeForage(void);

	//Main methods
	virtual int run();
	virtual int runStep();
	virtual string getAlgorithmName() { return "HoneyBee"; }

	virtual void initializeRobots();
	Coord randomRobotPosition();
};

