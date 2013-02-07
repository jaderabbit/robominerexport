#include "Experiment.h"


Experiment::Experiment(void)
{
}


Experiment::~Experiment(void)
{
}

Experiment::Experiment( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc ) : desc(_desc), env_desc(_env_desc), sampleCount(0), cnt(0){}

int Experiment::initialize() {
	initializeGrid(); //TODO: Have a "createEnvironmentFileName" method for a given environment descriptor.
	initializeSink();
	initializeRobots();

	//Initialize Performance measures
	pb = new PerformanceBed(robots);
	initializePerformanceMeasures();

	cnt = 0;
	return true;
}

int Experiment::cleanup() {
	//TODO: Certify that all clean up issues have been addressed. 
	robots.clear();
	mine.grid.clear();
	return true;
}

void Experiment::initializeGrid() {
	//Initialize the empty grid
	mine.initializeEmptyGrid(desc.width,desc.height);
	mine.load(desc, env_desc,"C:\\Users\\Jade\\Documents\\Visual Studio 2010\\Projects\\RoboMiner\\EnvironmentGenerator\\environments\\uniform\\uniform_size_50_obj_1250_ratio_0.666667_sim_14.txt");
}

void Experiment::initializeObjects() {
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

void Experiment::initializeSink() {
	//init sink
	mine.initSink();
}

