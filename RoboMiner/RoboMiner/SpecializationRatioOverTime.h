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

	virtual string getName() { return "SpecializationRatioOverTime" }

	virtual bool isNext();
	virtual string getNext();
	virtual double getNextValue();
	virtual double getFinalValue();
	virtual bool isPerIteration() {return true;}
	virtual int getId() {
		return 816;
	}
private:
	vector<double> running_ratio;
	vector<double> running_gold;
	vector<double> running_waste;
};

