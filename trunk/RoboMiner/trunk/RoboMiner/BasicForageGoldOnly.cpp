#include "BasicForageGoldOnly.h"
#include "BasicForagingState.h"
#include "ItemsForagedOverTime.h"
#include "Entropy.h"
#include "AverageTimeInState.h"
#include <sstream>

BasicForageGoldOnly::BasicForageGoldOnly(void)
{
}

BasicForageGoldOnly::~BasicForageGoldOnly(void)
{
}

BasicForageGoldOnly::BasicForageGoldOnly( EXPERIMENT_DESC _desc , ENVIRONMENT_DESC _env_desc ) : Experiment(_desc,_env_desc) {

}

int BasicForageGoldOnly::run() {

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

int BasicForageGoldOnly::runStep() {

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
			robots[i].setRobotState( new BasicForagingState( &robots[i], GOLD,1,1) );
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

int BasicForageGoldOnly::cleanup() {
	//TODO: Certify that all clean up issues have been addressed. 
	robots.clear();
	mine.grid.clear();
	return true;
}

void BasicForageGoldOnly::initializeRobots() {
	//Initialize Robots

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
		Robot r(&mine);
		r.setInitialPosition(p.x,p.y);
		r.setDir(d);
		r.setDivision(GOLD);
		r.setActivity(activity);
		r.setStringTracker(s.str());
		r.setMutualRobotAwareness(&robots);
		r.setIndex(i);
		r.setMaxPath(50);

		//Performance bed
		r.setPerformanceBed(pb);


		//push back robot
		robots.push_back(r);

	}

}

Coord BasicForageGoldOnly::randomRobotPosition() {
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

void BasicForageGoldOnly::initializePerformanceMeasures() {
	pb->attach( new ItemsForagedOverTime() );
	pb->attach( new AverageTimeInState(PM_FORAGE));
	pb->attach( new Entropy(desc.height,number_robots) );
 }
