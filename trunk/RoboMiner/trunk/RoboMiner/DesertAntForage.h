#pragma once
#include "experiment.h"
class DesertAntForage :
	public Experiment
{
public:
	DesertAntForage(void);
	DesertAntForage( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc  );
	virtual ~DesertAntForage(void);
	
	//Main methods
	virtual int run();
	virtual int runStep();

	void initializePerformanceMeasures();
	virtual void initializeRobots();
	virtual string getAlgorithmName() { return "DesertAnt"; }
	Coord randomRobotPosition();
};

