#pragma once

#include "Robot.h"
#include "PerformanceMeasure.h"

class PerformanceBed
{
public:
	PerformanceBed(void);
	virtual ~PerformanceBed(void);
	PerformanceBed( vector<Robot>& _robots);

	virtual void trigger();
	virtual void trigger( int index );

private:
	vector<Robot>& robots;
	vector<PerformanceMeasure> pm;

};

