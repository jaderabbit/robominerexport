#pragma once
#include "PerformanceMeasure.h"

enum {GOLD_M, WASTE_M, ALL_M, RUN_GOLD, RUN_WASTE };

class ItemsForagedOverTime : public PerformanceMeasure
{
public:
	ItemsForagedOverTime();
	ItemsForagedOverTime(int what);
	virtual ~ItemsForagedOverTime(void);

	virtual void takeMeasure( vector<Robot>& robots);

	virtual string getName() { return "ItemsForagedOverTime"; }

	virtual bool isNext();
	virtual string getNext();

private:
	vector<double> total;
	vector<double> gold;
	vector<double> running_total_gold;
	vector<double> waste;
	int time;
	int running_sum;
	int measureType;
};

