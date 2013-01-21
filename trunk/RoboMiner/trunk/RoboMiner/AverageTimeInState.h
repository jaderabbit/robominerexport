#pragma once
#include "PerformanceMeasure.h"

enum {PM_EXPLORE, PM_RETURN, PM_FORAGE, PM_WAIT, PM_RECRUIT, PM_OTHER };

class AverageTimeInState :
	public PerformanceMeasure
{
public:
	AverageTimeInState(void);
	AverageTimeInState( int _state);
	virtual ~AverageTimeInState(void);

	virtual void takeMeasure( vector<Robot>& robots);

	int getState(int _state);

	void finalize();

private:
	int state;
	int time;
	vector<int> timeSpentInStatePerRobot;
	vector<int> itemsForagedPerRobot;

	double finalStatistic;

};

