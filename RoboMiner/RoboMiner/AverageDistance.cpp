#include "AverageDistance.h"


AverageDistance::AverageDistance(void) : PerformanceMeasure()
{
	finalValue = true;
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

void AverageDistance::takeMeasure(vector<Robot>& robots) 
{
	
}

double AverageDistance::getNextValue() {
	//Return ratio at end
	return measurement[0];
}


double AverageDistance::getFinalValue() {
	//Return ratio at end
	return measurement[0];
}

