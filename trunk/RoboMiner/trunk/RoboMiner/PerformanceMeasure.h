#pragma once

#include "Robot.h"
class PerformanceMeasure
{
public:
	PerformanceMeasure( Robot& _robot);
	virtual ~PerformanceMeasure(void);

	virtual void takeMeasure( double measure) = 0;
protected:
	Robot& robot;
	vector<double> measurement;
};

