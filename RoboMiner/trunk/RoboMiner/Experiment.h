#pragma once

#include "EXPERIMENT_DESC.h"
#include "ENVIRONMENT_DESC.h"

#include "PerformanceBed.h"
#include "ResultWriter.h"
#include "Mine.h"
#include "Robot.h"

class Experiment
{
public:
	Experiment(void);
	Experiment(Tools &t);
	Experiment( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc);
	~Experiment(void);
	void setExperimentParam( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc);
	virtual void runAllSamples();

	virtual int initialize();
	virtual int run() = 0;
	virtual int runStep() = 0;
	virtual int getTotalIterations();
	virtual int runAllSamplesStep();
	virtual int cleanup();
	virtual void reset();

	//Initialization methods
	virtual void initializeGrid();
	virtual void initializeSink();
	virtual void initializeRobots() =0;
	virtual void initializePerformanceMeasures();
	virtual void addPerformanceMeasure(PerformanceMeasure * pm);

	virtual string getEnvironmentFileName();
	virtual string getAlgorithmName() = 0;

	//Environment
	Mine mine;
	vector<Robot> robots;
	int number_robots;

	//Performance bed
	PerformanceBed* pb;
	vector<PerformanceBed*> pbs;

	//Experiment desc
	EXPERIMENT_DESC desc;
	ENVIRONMENT_DESC env_desc;

	//Random tools
	Tools t;

	//Sample Count
	int samples;
	int sampleCount;

	//ResultWriter
	ResultWriter resultWriter;

	const static int SINK_BOUNDARY = 5;
	static const int MAX_WANDER_STEPS = 50;

	//Interation counter
	int cnt;
};

