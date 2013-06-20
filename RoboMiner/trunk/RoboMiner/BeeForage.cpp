#include "BeeForage.h"
#include "ItemsForagedOverTime.h"
#include "AverageTimeInState.h"
#include "Entropy.h"

#include <sstream>

BeeForage::BeeForage(void)
{
}

BeeForage::BeeForage( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc ) : Experiment(_desc,_env_desc) {
}


BeeForage::~BeeForage(void)
{
}

int BeeForage::run() {

	//FORAGE/EXPLORING
	for (unsigned int i=0; i < robots.size(); i++) {
		if ( i < number_robots*desc.forager_explorer_ratio ) 
			robots[i].setActivity(FORAGE);
		else {
			robots[i].setActivity(EXPLORE);
		}
	}
	cnt=0;

	while (cnt < desc.total_forage_iterations) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}

	return true;
}

int BeeForage::runStep() {
	//All robots initial activity is set at initialization

	//reset performance measures
	for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].resetPerformanceMeasures();
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


void BeeForage::initializeRobots() {
	
	int goldCount = ceil(number_robots*desc.gold_waste_division_ratio);
	int wasteCount = floor(number_robots - number_robots*desc.gold_waste_division_ratio);
	int forager_count = 0, explorer_count = 0; 	int c = 0;
	for (int i=0; i < number_robots ; i ++ ) {
		//Create Robots
		//choose position
		Coord p = randomRobotPosition();

		//generate random direction
		Coord d;
		d.x = t.random(-1,1);
		d.y = t.random(-1,1);
		
		//set position & index on board
		mine.setCell(p.x,p.y,ROBOT,i);

		//string stream for file name
		stringstream s;
		s << "tracker_robot_" << i << ".csv";
 
		//Construct robot
		Robot r(&mine);
		r.setInitialPosition(p.x,p.y);
		r.setDir(d);
		r.setStringTracker(s.str());
		r.setMutualRobotAwareness(&robots);
		r.setMaxPath(desc.max_path);
		r.setIndex(i);
		r.setLambda(0.5);

		//Performance bed
		r.setPerformanceBed(pb);

		//push back robot
		robots.push_back(r);

		//forager to explorer ratio
		if ( i <= (number_robots - number_robots*desc.forager_explorer_ratio )) {
			robots[i].setActivity(EXPLORE); //explorers need to alternate type
			explorer_count++;

			//Set GOLD WASTE division
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

		} else {
			robots[i].setActivity(FORAGE);
			forager_count++;
	
			//Set GOLD WASTE division
			if ( (goldCount > 0 && wasteCount > 0) || (goldCount == 0 && wasteCount == 0) ) {
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
		robots[i].setDivToOriginalDivision();
	}


}

Coord BeeForage::randomRobotPosition() {
	//choose position
	Coord p;

	//generate position located close to sink
	do { 
		int range = number_robots/env_desc.grid_size + 1;
		p.x = t.random(0,range); 
		p.y = t.random(0,env_desc.grid_size-1);
	} while (!mine.isEmpty(p.x,p.y));

	return p;

}