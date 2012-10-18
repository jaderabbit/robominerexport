#include "ItemsForagedOverTime.h"


ItemsForagedOverTime::ItemsForagedOverTime() 
{
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

}
