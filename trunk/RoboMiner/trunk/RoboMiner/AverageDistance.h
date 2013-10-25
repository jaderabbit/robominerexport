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
	virtual double getNextValue();

	virtual string getName() { return "AverageDistance"; }
	virtual bool isPerIteration() {return false;}
	virtual double getFinalValue();
	int time;
};

