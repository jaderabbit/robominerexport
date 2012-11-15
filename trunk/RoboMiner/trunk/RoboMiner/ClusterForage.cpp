#include "ClusterForage.h"
#include "ItemsForagedOverTime.h"
#include <sstream>

ClusterForage::ClusterForage(void)
{
}

ClusterForage::ClusterForage( EXPERIMENT_DESC _desc ) : Experiment(_desc) {
}


ClusterForage::~ClusterForage(void)
{
}

int ClusterForage::initialize() {
	initializeGrid();
	initializeSink();
	initializeObjects();
	initializeRobots();
	cnt = 0;
	return true;
}

int ClusterForage::run() {
	//CLUSTERING
	for (unsigned int i=0; i < robots.size(); i++) {
		robots[i].setActivity(CLUSTER);
	}

	while (cnt < desc.total_cluster_iterations) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}

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

int ClusterForage::runStep() {

	//reset performance measures
	for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].resetPerformanceMeasures();
	}

	//CLUSTERING
	if ( cnt == 0 ) {
		for (unsigned int i=0; i < robots.size(); i++) {
			robots[i].setActivity(CLUSTER);
		}
	}

	if (cnt < desc.total_cluster_iterations) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}

	if (cnt == desc.total_cluster_iterations) {
		int forager_count = 0, explorer_count = 0;
		for (unsigned int i=0; i < robots.size(); i++) {
			//Set random position by sink
			Coord p = randomRobotPosition();
			robots[i].setPosition(p.x,p.y);

			//forager to explorer ratio
			if ( i <= desc.number_robots*desc.forager_explorer_ratio ) {
				robots[i].setActivity(FORAGE);
				forager_count++;
			} else {
				robots[i].setActivity(EXPLORE); //explorers need to alternate type
				explorer_count++;
			}
		}

		for (unsigned int i=0; i < forager_count; i++) {
			int division = ( i <= desc.gold_waste_division_ratio_forage*forager_count) ? GOLD : WASTE;
			robots[i].setDivision(division);
		}

		for (unsigned int i=forager_count; i < explorer_count; i++) {
			int division = ( i <= desc.gold_waste_division_ratio_forage*explorer_count) ? GOLD : WASTE;
			robots[i].setDivision(division);
		}

	}

	if ( cnt >= desc.total_cluster_iterations && cnt < desc.total_cluster_iterations + desc.total_forage_iterations) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}

	//Add performance measure trigger ALL
	pb->trigger();

	return true;
}

int ClusterForage::cleanup() {
	//TODO: Certify that all clean up issues have been addressed. 
	robots.clear();
	mine.grid.clear();
	return true;
}

void ClusterForage::initializeGrid() {
	//Initialize the empty grid
	mine.initializeEmptyGrid(desc.width,desc.height);
}

void ClusterForage::initializeObjects() {
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

void ClusterForage::initializeRobots() {
	//Initialize Robots
	pb = new PerformanceBed(robots);
	pb->attach( new ItemsForagedOverTime() );

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
		int activity = CLUSTER;

		//Division
		int division = ( i < desc.gold_waste_division_ratio_cluster*desc.number_robots) ? GOLD : WASTE;

		//string stream for file name
		stringstream s;
		s << "tracker_robot_" << i << ".csv";
 
		//Construct robot
		Robot r(&mine);
		r.setInitialPosition(p.x,p.y);
		r.setDir(d);
		r.setDivision(division);
		r.setActivity(CLUSTER);
		r.setStringTracker(s.str());
		r.setMutualRobotAwareness(&robots);
		r.setIndex(i);

		//Performance bed
		r.setPerformanceBed(pb);

		//push back robot
		robots.push_back(r);

	}

}

void ClusterForage::initializeSink() {
	//init sink
	mine.initSink();
}

Coord ClusterForage::randomRobotPosition() {
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
