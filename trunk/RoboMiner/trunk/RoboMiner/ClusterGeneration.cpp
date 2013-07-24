#include "ClusterGeneration.h"

ClusterGeneration::ClusterGeneration(void) : Experiment()
{
}

ClusterGeneration::ClusterGeneration(Tools &_t) : Experiment(_t)
{
}

ClusterGeneration::~ClusterGeneration(void)
{
}

ClusterGeneration::ClusterGeneration( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc  ) : Experiment(_desc,_env_desc) {}


int ClusterGeneration::run() {
	//Generate ants 
	for (unsigned int i=0; i < robots.size(); i++) {
		//forager to explorer ratio
		robots[i].setActivity(CLUSTER);
		robots[i].setState(UNLOADED);
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

int ClusterGeneration::runStep() {
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

void ClusterGeneration::initializeRobots() {
	for (int i=0; i < number_robots ; i ++ ) {
		//Create Robots
		//choose position
		Coord p = randomRobotPosition();

		//generate random direction
		Coord d;
		d.x = t.random(-1,1);
		d.y = t.random(-1,1);
		
		//set position & index on board
		mine.setCell(p.x,p.y,mine.grid[p.x][p.y].type,i);

		//string stream for file name
		stringstream s;
		s << "tracker_robot_" << i << ".csv";
 
		//Construct robot
		Robot r(&mine,t);
		r.setInitialPosition(p.x,p.y);
		r.setDir(d);
		r.setStringTracker(s.str());
		r.setMutualRobotAwareness(&robots);
		r.setMaxPath(desc.max_path);
		r.setIndex(i);
		r.setLambda(0.5);
		r.setActivity(CLUSTER);
		r.setState(UNLOADED);

		//Performance bed
		r.setPerformanceBed(pb);

		//push back robot
		robots.push_back(r);

		if ( t.randomOpen() <= desc.gold_waste_division_ratio ) {
			robots[i].setDivision(GOLD);
		} else {
			robots[i].setDivision(WASTE);
		}
	}
}


Coord ClusterGeneration::randomRobotPosition() {
	//choose position
	Coord p;

	//generate position somewhre on grid;
	p.x = t.random(0,desc.width-1); 
	p.y = t.random(0,desc.height-1);

	return p;

}

int ClusterGeneration::initialize() {
	initializeGrid(); //TODO: Have a "createEnvironmentFileName" method for a given environment descriptor.
	//initializeSink();

	number_robots = ceil((desc.number_robots*1.0/100.0)*env_desc.grid_size);
	initializeRobots();

	//Initialize Performance measures
	pb = new PerformanceBed(robots);

	for (int i=0; i < robots.size(); i++) {
		robots[i].setPerformanceBed(pb);
	}
	initializePerformanceMeasures();

	cnt = 0;
	return true;
}
