#pragma once

#include "PerformanceBed.h"
#include "Mine.h"
#include "Robot.h"

class Experiment
{
public:
	Experiment(void);
	~Experiment(void);

	virtual int initialize() = 0;
	virtual int run() = 0;
	virtual int runStep() = 0;
	virtual int cleanup() = 0;

	Mine mine;
	vector<Robot> robots;

	PerformanceBed* pb;
};

