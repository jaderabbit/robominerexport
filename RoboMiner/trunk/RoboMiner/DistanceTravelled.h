#pragma once
#include "performancemeasure.h"
class DistanceTravelled //:
	//public PerformanceMeasure
{
public:
	DistanceTravelled(void);
	virtual ~DistanceTravelled(void);
	virtual void takeMeasure( vector<Robot>& robots);

	virtual string getName() { return "DistanceTravelled"; }

	virtual bool isNext();
	virtual string getNext();
};

