#include "BasicForageGoldOnly.h"
#include "BasicForagingState.h"
#include "ItemsForagedOverTime.h"
#include "AverageTimeInState.h"
#include <sstream>

BasicForageGoldOnly::BasicForageGoldOnly(void)
{
}

BasicForageGoldOnly::~BasicForageGoldOnly(void)
{
}

BasicForageGoldOnly::BasicForageGoldOnly( EXPERIMENT_DESC _desc ) : Experiment(_desc) {

}

int BasicForageGoldOnly::initialize() {
	initializeGrid();
	initializeSink();
	initializeObjects();
	initializeRobots();

	//Initialize Performance measures
	pb = new PerformanceBed(robots);
	pb->attach( new ItemsForagedOverTime() );
	pb->attach( new AverageTimeInState(PM_FORAGE));

	cnt = 0;
	return true;
}

int BasicForageGoldOnly::run() {
	
	for (unsigned int i=0; i < robots.size(); i++) {
		//Set Foraging
		robots[i].setActivity(BASICFORAGE);

		//Set states
		robots[i].setRobotState( new BasicForagingState( &robots[i], GOLD ) );
	}

	while (cnt < desc.total_forage_iterations) {
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

	//CLUSTERING
	if ( cnt == 0 ) {
		for (unsigned int i=0; i < robots.size(); i++) {
			//Set Foraging
			robots[i].setActivity(BASICFORAGE);

			//Set states
			robots[i].setRobotState( new BasicForagingState( &robots[i], GOLD,1,1) );
		}

	}

	if (cnt < desc.total_cluster_iterations) {
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

void BasicForageGoldOnly::initializeGrid() {
	//Initialize the empty grid
	mine.initializeEmptyGrid(desc.width,desc.height);
}

void BasicForageGoldOnly::initializeObjects() {
	//Randomly
	for (int i=0; i < desc.number_objects; i++) {
		bool empty = false;
		while (!empty) {
			//generate random position.
			int x_new = t.random(SINK_BOUNDARY,desc.width-1);
			int y_new = t.random(0,desc.height-1);

			//set new position
			if ( i < (desc.number_objects*desc.gold_waste_ratio) )
				empty = mine.setCellIfEmpty(x_new, y_new,GOLD);
			else 
				empty = mine.setCellIfEmpty(x_new, y_new,WASTE);
		}
	}
}

void BasicForageGoldOnly::initializeRobots() {
	//Initialize Robots

	//TODO: Decide if initialize at sink or randomly
	//TODO: Allow for robots to NOT choose what items to cluster. i.e. A robot can cluster any item.
	for (int i=0; i < desc.number_robots ; i ++ ) {
		//choose position
		Coord p = randomRobotPosition();

		//generate random direction
		Coord d;
		d.x = t.random(-1,1);
		d.y = t.random(-1,1);
		
		//set position & index on board
		mine.setCell(p.x,p.y,ROBOT,i);
				
		//All need to cluster 
		int activity = BASICFORAGE;

		//string stream for file name
		stringstream s;
		s << "tracker_robot_" << i << ".csv";
 
		//Construct robot
		Robot r(&mine);
		r.setInitialPosition(p.x,p.y);
		r.setDir(d);
		r.setDivision(GOLD);
		r.setActivity(BASICFORAGE);
		r.setStringTracker(s.str());
		r.setMutualRobotAwareness(&robots);
		r.setIndex(i);

		//Performance bed
		r.setPerformanceBed(pb);

		r.setMaxPath(50);

		//push back robot
		robots.push_back(r);

	}

}

void BasicForageGoldOnly::initializeSink() {
	//init sink
	mine.initSink();
}

Coord BasicForageGoldOnly::randomRobotPosition() {
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

