#pragma once
#include "Experiment.h"
class BasicForage :
	public Experiment
{
public:
	BasicForage(void);
	BasicForage(Tools &_t);
	BasicForage(EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc );
	virtual ~BasicForage(void);

	virtual int run();
	virtual int runStep();
	virtual int cleanup();

	//Initialize
	virtual void initializeRobots();
	virtual string getAlgorithmName() { return "Basic"; }
	//Chose robot position
	Coord randomRobotPosition();

	const static int SINK_BOUNDARY = 5;
	static const int MAX_WANDER_STEPS = 50;
};

