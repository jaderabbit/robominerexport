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
	Experiment( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc);
	~Experiment(void);

	virtual int runAllSamplesStep() = 0;
	virtual int initialize() = 0;
	virtual int run() = 0;
	virtual int runStep() = 0;
	virtual int cleanup() = 0;

	//Environment
	Mine mine;
	vector<Robot> robots;

	//Performance bed
	PerformanceBed* pb;
	vector<PerformanceBed*> pbs;

	//Experiment desc
	EXPERIMENT_DESC desc;
	ENVIRONMENT_DESC env_desc;

	//Random tools
	Tools t;

	//Sample Count
	static const int samples = 30;
	int sampleCount;

	//ResultWriter
	ResultWriter resultWriter;
};

