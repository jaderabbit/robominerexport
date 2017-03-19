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

	virtual string getName() { 
		string t = (measureType == RUN_GOLD) ? "_GOLD" : "_WASTE";
		return "ItemsForagedOverTime" + t;
	}

	virtual bool isNext();
	virtual string getNext();
	virtual double getNextValue();
	virtual double getFinalValue();
	virtual bool isPerIteration() {return true;}
	virtual int getId() {
		if (measureType == RUN_GOLD)
		{
			return 1;
		} else 
		{
			return 2;
		}
	}
private:
	vector<double> total;
	vector<double> gold;
	vector<double> running_total_gold;
	vector<double> waste;
	int time;
	int running_sum;
	int measureType;
	double absoluteTotal;
};

