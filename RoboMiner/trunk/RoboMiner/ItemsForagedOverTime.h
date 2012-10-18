#pragma once
#include "PerformanceMeasure.h"

class ItemsForagedOverTime : public PerformanceMeasure
{
public:
	ItemsForagedOverTime();
	virtual ~ItemsForagedOverTime(void);

	virtual void takeMeasure( vector<Robot>& robots);

	vector<double> total;
	vector<double> gold;
	vector<double> waste;
};

