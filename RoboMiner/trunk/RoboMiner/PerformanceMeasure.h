#pragma once

#include "Robot.h"
class PerformanceMeasure
{
public:
	PerformanceMeasure();
	virtual ~PerformanceMeasure(void);

	virtual void takeMeasure( Robot& robot) = 0;
protected:

	vector<double> measurement;
};

