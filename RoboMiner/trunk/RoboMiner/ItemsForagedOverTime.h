#pragma once
#include "PerformanceMeasure.h"

class ItemsForagedOverTime : public PerformanceMeasure
{
public:
	ItemsForagedOverTime( Robot& _robot);
	virtual ~ItemsForagedOverTime(void);

	virtual void takeMeasure( double measure );

};

