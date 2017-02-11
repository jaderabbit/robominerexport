#pragma once

#include "Robot.h"
#include "PerformanceMeasure.h"

class PerformanceBed
{
public:
	virtual ~PerformanceBed(void);
	PerformanceBed( const PerformanceBed& other );
	PerformanceBed& operator=(const PerformanceBed&);
	PerformanceBed( vector<Robot>& _robots);

	virtual void trigger();
	virtual void attach( PerformanceMeasure* _pm );
	virtual void finalize();


	vector<Robot>& robots;
	vector<PerformanceMeasure*> pm;

};

