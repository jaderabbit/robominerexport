#pragma once
#include "performancemeasure.h"
class AverageDistance :
	public PerformanceMeasure
{
public:
	AverageDistance(void);
	virtual ~AverageDistance(void);

	virtual bool isNext();
	virtual string getNext();

	virtual string getName() { return "AverageDistance"; }

	int time;
};

