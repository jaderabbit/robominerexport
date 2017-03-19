#pragma once
#include "PerformanceMeasure.h"

enum {PM_EXPLORE, PM_RETURN, PM_FORAGE, PM_WAIT, PM_RECRUIT, PM_OTHER, PM_CHOOSE_ACTIVITY };

class AverageTimeInState :
	public PerformanceMeasure
{
public:
	AverageTimeInState(void);
	AverageTimeInState( int _state);
	virtual ~AverageTimeInState(void);

	virtual void takeMeasure( vector<Robot>& robots);
	virtual string getName();

	int getState(int _old_state);
	string getStateName( int _state);
	void finalize();

	virtual bool isNext();
	virtual string getNext();
	virtual double getNextValue();
	virtual double getFinalValue();
	virtual bool isPerIteration() {return false;}
	virtual int getId(){return 3;}
private:
	int state;
	int time;
	vector<int> timeSpentInStatePerRobot;
	vector<int> itemsForagedPerRobot;
	int totalItemsForaged;
	int totalTimeSpentInState;
	double finalStatistic;
	int num_robots;

};

