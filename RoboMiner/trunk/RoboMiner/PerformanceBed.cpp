#include "PerformanceBed.h"

PerformanceBed::PerformanceBed( vector<Robot> &_robots) : robots(_robots) {

}


PerformanceBed::PerformanceBed(void)
{
}


PerformanceBed::~PerformanceBed(void)
{
}

 void PerformanceBed::trigger() 
 {
	 //for each robot
	 for (int j=0; j < robots.size(); j++ ) 	 
	 {
		 //for each performance measure
		 for (int i(0); i < pm.size(); i++) {
			pm[i].takeMeasure(robots[j]);
		 }
	 }
 }

 void PerformanceBed::trigger( int index ) 
 {
	//for each performance measure
	for (int i(0); i < pm.size(); i++) {
		pm[i].takeMeasure(robots[index]);
	}
 }
