#include "ItemsForagedOverTime.h"
#include <sstream>

ItemsForagedOverTime::ItemsForagedOverTime() : PerformanceMeasure()
{
	time = 0;
}


ItemsForagedOverTime::~ItemsForagedOverTime(void)
{
}

void ItemsForagedOverTime::takeMeasure( vector<Robot>& robots) 
{
	int sum = 0;
	int goldSum = 0;
	int wasteSum = 0;

	for (int i=0; i < robots.size(); i++) {
		sum = ( robots[i].foraged ) ? sum+1 : sum;
		goldSum = ( robots[i].foraged && robots[i].typeForaged == GOLD ) ? goldSum+1 : goldSum;
		wasteSum = ( robots[i].foraged && robots[i].typeForaged == WASTE ) ? wasteSum+1 : wasteSum;
	}

	total.push_back(sum);
	gold.push_back(goldSum);
	waste.push_back(wasteSum);
	time++;
}

bool ItemsForagedOverTime::isNext() {
	return ( timer > total.size() ) ? false : true;
}

string ItemsForagedOverTime::getNext() {
	if (timer >= total.size() ) {
		timer++;
		return "";
	}
	
	stringstream s1;
	s1 << total[timer];
	timer++;
	return s1.str();
}