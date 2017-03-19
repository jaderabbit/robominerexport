#include "SpecializationRatioOverTime.h"
#include "Mine.h"
#include <sstream>

SpecializationRatioOverTime::SpecializationRatioOverTime() : PerformanceMeasure()
{
	time = 0;
}


SpecializationRatioOverTime::SpecializationRatioOverTime(int type) : PerformanceMeasure()
{
	time = 0;
}


SpecializationRatioOverTime::~SpecializationRatioOverTime(void)
{
}

void SpecializationRatioOverTime::takeMeasure( vector<Robot>& robots) 
{
  int gold_total = 0;
  int waste_total = 0;

  for (unsigned int i=0; i < robots.size(); i++) 
  {
    if (robots[i].division == GOLD)
    {
      gold_total++;
    }
    else 
    {
      waste_total++;
    }
  }

  running_gold.push_back(gold_total);
  running_waste.push_back(waste_total);
  double ratio = (gold_total*1.0)/(robots.size()*1.0);
  running_ratio.push_back(ratio);
	time++;
}

bool SpecializationRatioOverTime::isNext() {
	return ( timer >= running_ratio.size() ) ? false : true;
}

string SpecializationRatioOverTime::getNext() {
	//Return ratio at end
	if (timer >= running_ratio.size() ) {
		stringstream s1;
		s1 << running_ratio[running_ratio.size()-1];
		timer++;
		return s1.str();
	}
	
	stringstream s1;
	s1 << running_ratio[timer];
	timer++;
	return s1.str();
}

double SpecializationRatioOverTime::getNextValue() {
	//Return ratio at end
	if (timer >= running_ratio.size() ) {
		return running_ratio[running_ratio.size()-1];
		timer++;
	}
	
	double val  = running_ratio[timer];
	timer++;
	return val;
}

double SpecializationRatioOverTime::getFinalValue() {
	//Return ratio at end
	return ( running_ratio.size() > 0 ) ?  running_ratio[running_ratio.size()-1] : 0;
}