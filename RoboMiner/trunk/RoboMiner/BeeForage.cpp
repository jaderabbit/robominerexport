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
		if ( i < desc.number_robots*desc.forager_explorer_ratio ) 
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

 void BeeForage::initializePerformanceMeasures() {
	pb->attach( new ItemsForagedOverTime() );
	pb->attach( new AverageTimeInState(PM_FORAGE));
	pb->attach( new Entropy(desc.height,desc.number_robots) );
	pb->attach( new ItemsForagedOverTime() );

 }

void BeeForage::initializeRobots() {
	int forager_count = 0, explorer_count = 0; 	int c = 0;
	for (int i=0; i < desc.number_robots ; i ++ ) {
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
		if ( i <= desc.number_robots*desc.forager_explorer_ratio ) {
			robots[i].setActivity(FORAGE);
			forager_count++;
		} else {
			robots[i].setActivity(EXPLORE); //explorers need to alternate type
			explorer_count++;
		}

		//Set Division ratio
		if ( i <= desc.number_robots*desc.gold_waste_division_ratio ) {
			if ( c % 2 == 0 )
				robots[i].setDivision(GOLD);
			else 
				robots[i].setDivision(WASTE);
		} else {
			if ( c % 2 == 0 )
				robots[i].setDivision(WASTE);
			else 
				robots[i].setDivision(GOLD);
		}
	}

}

Coord BeeForage::randomRobotPosition() {
	//choose position
	Coord p;

	//generate position located close to sink
	do { 
		int range = desc.number_robots/desc.width + 1;
		p.x = t.random(0,range); 
		p.y = t.random(0,desc.height-1);
	} while (!mine.isEmpty(p.x,p.y));

	return p;

}