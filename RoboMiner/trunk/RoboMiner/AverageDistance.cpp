#include "AverageDistance.h"


AverageDistance::AverageDistance(void) : PerformanceMeasure()
{
}


AverageDistance::~AverageDistance(void)
{
}


bool AverageDistance::isNext() {
	return ( timer > time ) ? false : true;
}

string AverageDistance::getNext() {
	stringstream s;
	if (timer < time ) {
		timer++;
		return "";
	} else {		
		s.clear();
		s << measurement[0];
		timer++;
		return s.str();
	}
}

double AverageDistance::getNextValue() {
	//Return ratio at end
	return measurement[0];
}


double AverageDistance::getFinalValue() {
	//Return ratio at end
	return measurement[0];
}
