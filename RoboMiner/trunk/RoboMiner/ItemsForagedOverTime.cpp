#include "ItemsForagedOverTime.h"
#include <sstream>

ItemsForagedOverTime::ItemsForagedOverTime() : PerformanceMeasure()
{
	time = 0;
	measureType = RUN_GOLD;
	running_sum = 0;
}


ItemsForagedOverTime::ItemsForagedOverTime(int type) : PerformanceMeasure()
{
	time = 0;
	measureType = type;
	running_sum = 0;
}


ItemsForagedOverTime::~ItemsForagedOverTime(void)
{
}

void ItemsForagedOverTime::takeMeasure( vector<Robot>& robots) 
{
	int sum = 0;
	int goldSum = 0;
	int wasteSum = 0;
	for (unsigned int i=0; i < robots.size(); i++) {
		sum = ( robots[i].foraged ) ? sum+1 : sum;
		goldSum = ( robots[i].foraged && robots[i].typeForaged == GOLD ) ? goldSum+1 : goldSum;
		wasteSum = ( robots[i].foraged && robots[i].typeForaged == WASTE ) ? wasteSum+1 : wasteSum;
	}

	switch(measureType) {
		case GOLD_M: total.push_back(goldSum); break;
		case WASTE_M: total.push_back(wasteSum); break;
		case ALL_M:total.push_back(sum); break;
		case RUN_GOLD: running_sum+= goldSum; total.push_back(running_sum); break;
		case RUN_WASTE:running_sum+= wasteSum; total.push_back(running_sum); break;
		default: total.push_back(goldSum); break;
	}

	time++;
}

bool ItemsForagedOverTime::isNext() {
	return ( timer > total.size() ) ? false : true;
}

string ItemsForagedOverTime::getNext() {
	if (timer >= total.size() ) {
		stringstream s1;
		s1 << total[total.size()-1];
		timer++;
		return s1.str();
	}
	
	stringstream s1;
	s1 << total[timer];
	timer++;
	return s1.str();
}