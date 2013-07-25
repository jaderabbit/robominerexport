#pragma once

#include "Robot.h"
#include <sstream>

class PerformanceMeasure
{
public:
	PerformanceMeasure();
	virtual ~PerformanceMeasure(void);
	PerformanceMeasure( const PerformanceMeasure& other );
	PerformanceMeasure& operator=(const PerformanceMeasure&);
	virtual void takeMeasure( vector<Robot>& robots) = 0;
	virtual void finalize() {}
//Methods required for outputting results
	virtual bool isNext() = 0;
	virtual string getNext() = 0;
	virtual string getName() = 0;
	//virtual void reset() = 0;

protected:

	vector<double> measurement;
	int timer;
	//stringstream s; 
};

