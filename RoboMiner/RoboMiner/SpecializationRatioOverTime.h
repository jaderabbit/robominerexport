#pragma once
#include "PerformanceMeasure.h"
class SpecializationRatioOverTime : public PerformanceMeasure
{
public:
	SpecializationRatioOverTime();
	SpecializationRatioOverTime(int what);
	virtual ~SpecializationRatioOverTime(void);

	virtual void takeMeasure( vector<Robot>& robots);

	virtual string getName() { return "SpecializationRatioOverTime"; }

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
	int time;
};

