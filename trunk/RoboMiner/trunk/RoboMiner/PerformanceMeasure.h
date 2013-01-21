#pragma once

#include "Robot.h"
class PerformanceMeasure
{
public:
	PerformanceMeasure();
	virtual ~PerformanceMeasure(void);

	virtual void takeMeasure( vector<Robot>& robots) = 0;
	virtual void finalize() {}
protected:

	vector<double> measurement;
};

