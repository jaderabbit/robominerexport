#pragma once
#include "PerformanceMeasure.h"

class ItemsForagedOverTime : public PerformanceMeasure
{
public:
	ItemsForagedOverTime();
	virtual ~ItemsForagedOverTime(void);

	virtual void takeMeasure( vector<Robot>& robots);

	virtual string getName() { return "ItemsForagedOverTime"; }
private:
	vector<double> total;
	vector<double> gold;
	vector<double> waste;
	int time;
};

