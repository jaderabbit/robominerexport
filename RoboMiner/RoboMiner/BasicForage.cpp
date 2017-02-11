#include "BasicForage.h"
#include "BasicForagingState.h"
#include "ItemsForagedOverTime.h"
#include "Entropy.h"
#include "AverageTimeInState.h"
#include <sstream>

BasicForage::BasicForage(void) :Experiment()
{
}

BasicForage::BasicForage(Tools &_t) : Experiment(_t)
{
}

BasicForage::~BasicForage(void)
{
}

BasicForage::BasicForage( EXPERIMENT_DESC _desc , ENVIRONMENT_DESC _env_desc ) : Experiment(_desc,_env_desc) {

}

int BasicForage::run() {

	for (unsigned int i=0; i < robots.size(); i++) {
		//Set Foraging
		robots[i].setActivity(BASICFORAGE);

		//Set states
		robots[i].setRobotState( new BasicForagingState( &robots[i], GOLD ) );
	}

	while (cnt < desc.total_iterations) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}

	return true;
}

int BasicForage::runStep() {

	//reset performance measures
	for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].resetPerformanceMeasures();
	}

	//Foraging
	if ( cnt == 0 ) {
		for (unsigned int i=0; i < robots.size(); i++) {
			//Set Foraging
			robots[i].setActivity(BASICFORAGE);

			//Set states
			if ( t.randomOpen() <= desc.gold_waste_division_ratio ) 
				robots[i].setRobotState( new BasicForagingState( &robots[i], GOLD,1,1) );
			else 
				robots[i].setRobotState( new BasicForagingState( &robots[i], WASTE,1,1) );
		}
	}

	if (cnt < desc.total_iterations) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}

	//Add performance measure trigger ALL
	pb->trigger();

	return true;
}

int BasicForage::cleanup() {
	//TODO: Certify that all clean up issues have been addressed. 
	robots.clear();
	mine.grid.clear();
	return true;
}

void BasicForage::initializeRobots() {
	//Initialize Robots
	int goldCount = ceil(number_robots*desc.gold_waste_division_ratio);
	int wasteCount = floor(number_robots - number_robots*desc.gold_waste_division_ratio);
	//TODO: Decide if initialize at sink or randomly
	//TODO: Allow for robots to NOT choose what items to cluster. i.e. A robot can cluster any item.
	for (int i=0; i < number_robots ; i ++ ) {
		//choose position
		Coord p = randomRobotPosition();

		//generate random direction
		Coord d;
		d.x = t.random(-1,1);
		d.y = t.random(-1,1);
		
		//set position & index on board
		mine.setCell(p.x,p.y,ROBOT,i);
				
		//All need to cluster 
		int activity = FORAGE;

		//string stream for file name
		stringstream s;
		s << "tracker_robot_" << i << ".csv";
 
		//Construct robot
		Robot r(&mine,t);
		r.setInitialPosition(p.x,p.y);
		r.setDir(d);

		
		r.setActivity(activity);
		r.setStringTracker(s.str());
		r.setMutualRobotAwareness(&robots);
		r.setIndex(i);
		r.setMaxPath(50);

		//Performance bed
		r.setPerformanceBed(pb);

		//push back robot
		robots.push_back(r);

		if (desc.gold_waste_division_ratio < 1 && desc.gold_waste_division_ratio > 0 && i==0 ) { 
			//if there ratio isnt only gold or only waste and is first. 
			//This is to ensure there exists AT LEAST one gold forager.
			robots[i].setDivision(GOLD);
			goldCount--;
		} else if (desc.gold_waste_division_ratio < 1 && desc.gold_waste_division_ratio > 0 && i== 1) {  
			//if there ratio isnt only gold or only waste and is second
			//if there ratio isnt only gold or only waste and is first. 
			//This is to ensure there exists AT LEAST one waste forager.
			robots[i].setDivision(WASTE);
			wasteCount--;
		} else if ( (goldCount > 0 && wasteCount > 0) || (goldCount == 0 && wasteCount == 0) ) {
			//randomly choose
			if (t.orand() < 0.5 ) {
				robots[i].setDivision(GOLD);
				goldCount--;
			} else {
				robots[i].setDivision(WASTE);
				wasteCount--;
			}
		} else if ( goldCount > 0 && wasteCount == 0 ) {
			robots[i].setDivision(GOLD);
			goldCount--;
			//choose Gold
		} else if ( goldCount == 0 && wasteCount > 0 ) {
			//choose waste
			robots[i].setDivision(WASTE);
			wasteCount--;
		} 


	}
}

Coord BasicForage::randomRobotPosition() {
	//choose position
	Coord p;

	//generate position located close to sink
	do { 
		int range = number_robots/desc.width + 1;
		p.x = t.random(0,range); 
		p.y = t.random(0,desc.height-1);
	} while (!mine.isEmpty(p.x,p.y));

	return p;
}