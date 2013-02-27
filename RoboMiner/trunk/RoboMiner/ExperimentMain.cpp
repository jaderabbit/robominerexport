#include "BeeForage.h"
#include "BasicForageGoldOnly.h"
#include <vector>

int main() {

	//Mine
	Mine m; 

	//Experiment
	Experiment* experiment;

	//Setup Experiment
	EXPERIMENT_DESC d;
	d.width = 50; d.height = 50;
	d.number_objects = 700;
	d.number_robots = 20;
	d.gold_waste_ratio = 0.4;
	d.forager_explorer_ratio = 0.7;
	d.total_iterations = 4000;
	d.gold_waste_division_ratio = 0.5;
	d.max_path = 50;
	d.samples = 19;

	ENVIRONMENT_DESC e;
	e.grid_size = 50;
	e.num_objects = 20;
	e.ratio_gold = 0.2;
	e.type = "clustered";
	e.sink_boundary = 5;
	experiment = new BeeForage(d,e);

	//Initialize Experiment
	experiment->initialize();

	//for experiment duration	
		//Perform updates
		experiment->runAllSamplesStep();
}