#pragma once
#include "performancemeasure.h"
class AverageDistance :
	public PerformanceMeasure
{
public:
	AverageDistance(void);
	virtual ~AverageDistance(void);

	virtual string getName() { return "AverageDistance"; }
};

