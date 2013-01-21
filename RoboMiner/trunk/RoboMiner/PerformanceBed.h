#pragma once

#include "Robot.h"
#include "PerformanceMeasure.h"

class PerformanceBed
{
public:
	virtual ~PerformanceBed(void);
	PerformanceBed( vector<Robot>& _robots);

	virtual void trigger();
	virtual void attach( PerformanceMeasure* _pm );
	virtual void finalize();

private:
	vector<Robot>& robots;
	vector<PerformanceMeasure*> pm;

};

