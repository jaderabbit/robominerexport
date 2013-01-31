#pragma once

#include "Tools.h"
#include "experiment.h"

class ClusterForage :
	public Experiment
{
public:
	ClusterForage(void);
	ClusterForage( EXPERIMENT_DESC _desc );
	virtual ~ClusterForage(void);

	//Main methods
	virtual int initialize();
	virtual int run();
	virtual int runStep();
	virtual int runAllSamplesStep();
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



	//Interation counter
	int cnt;

};

