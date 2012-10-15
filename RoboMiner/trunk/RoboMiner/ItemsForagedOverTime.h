#pragma once
#include "PerformanceMeasure.h"

class ItemsForagedOverTime : public PerformanceMeasure
{
public:
	ItemsForagedOverTime();
	virtual ~ItemsForagedOverTime(void);

	virtual void takeMeasure( double measure );

};

