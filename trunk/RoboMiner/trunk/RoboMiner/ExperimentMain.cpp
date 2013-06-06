#include "BeeForage.h"
#include "BasicForageGoldOnly.h"
#include "BasicForage.h"
#include "Experiment.h"
#include <process.h>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include <WinBase.h>
//#include <Processthreadsapi.h>

using namespace std;



bool runExperiment( Experiment* e, EXPERIMENT_DESC exp_desc, ENVIRONMENT_DESC env_desc ) {
	//Set the experiment parameters
	e->setExperimentParam(exp_desc,env_desc);

	//Initialize
	e->initialize();

	//Run all samples
	e->runAllSamples();

	return true;
}

void beginThread( void* pParams ) {
	ENVIRONMENT_DESC env_desc;
	
    //Descriptors
	EXPERIMENT_DESC exp_desc;

	//Variables that do not change.
	exp_desc.max_path = 50;				//Max path
	exp_desc.samples = 30;				//Samples
	exp_desc.total_iterations = 4000;	//total iterations
	env_desc.sink_boundary = 5;			//Sink boundary
	
	//--------------Environment parameters. Outermost loops-------------------------

	//Environment types
	string environment_types[] = { "clustered", "uniform", "gaussian", "vein" };
	int num_environment_types = 4;
	int index = *(int*) pParams;
	env_desc.type=  environment_types[index];


	//Grid sizes
	int grid_sizes[] = { 50, 100, 200, 300, 500 };
	vector<int> gridSize (grid_sizes, grid_sizes + sizeof(grid_sizes) / sizeof(int) );

	//Object percentage
	int percentage_objects[] = { 5, 20, 50, 70, 90 };
	vector<int> objectPercentage (percentage_objects, percentage_objects + sizeof(percentage_objects) / sizeof(int) );

	//Gold waste ratios
	double gold_ratios[] = { 0, 0.2, 0.25, 0.33333333, 0.5, 0.666666667, 0.75, 0.8, 1 };
	vector<double> goldWasteRatio (gold_ratios, gold_ratios + sizeof(gold_ratios) / sizeof(int) );

	//-------------Experiment parameters. Innermost loops----------------------
	//Number of robots
	int num_robots[] = { 10 };
	vector<int> numRobots (num_robots, num_robots + sizeof(num_robots) / sizeof(int) );

	//Experiments
	vector<Experiment*> experiments;
	experiments.push_back( new BeeForage() );
	experiments.push_back( new BasicForage() );

	//Forager explorer ratio
	double forager_explorer_ratio[] = { 0.1, 0.3, 0.5, 0.7, 0.9 };
	vector<double> foragerExplorerRatio (forager_explorer_ratio, forager_explorer_ratio + sizeof(forager_explorer_ratio) / sizeof(int) );

	//Gold waste division ratio
	double gold_waste_robot_division[] = {  0, 0.2, 0.25, 0.33333333, 0.5, 0.666666667, 0.75, 0.8, 1  };
	vector<double> goldWasteRobotDiv (gold_waste_robot_division, gold_waste_robot_division + sizeof(gold_waste_robot_division) / sizeof(int) );

	for (int j=0; j < gridSize.size(); j++) {
			env_desc.grid_size = gridSize[j];

			//Percentage of objects
			for (int k=0; k < objectPercentage.size(); k++ ) {
				env_desc.num_objects = objectPercentage[k];

				//Gold to waste ratio
				for (int p=0; p < goldWasteRatio.size(); p++) {
					env_desc.ratio_gold = goldWasteRatio[p];

					//Experiment
					//Number of robots
					for (int q = 0; q < numRobots.size(); q++) {
						exp_desc.number_robots = numRobots[q];

						//Forager explorer ratio
						for (int u=0; u < foragerExplorerRatio.size(); u++) {		
							exp_desc.forager_explorer_ratio = foragerExplorerRatio[u];

							//Gold waste robot division
							for (int y=0; y < goldWasteRobotDiv.size(); y++) {		
								exp_desc.gold_waste_division_ratio = goldWasteRobotDiv[y];

								//Experiment
								for (int v = 0; v < experiments.size(); v++ ) {
									 runExperiment( experiments[v], exp_desc, env_desc);
								}
							}

						}

					}

				}
			}
		}
}

LPTSTR createCommandlineString( ENVIRONMENT_DESC e, EXPERIMENT_DESC d ) {

}

int main(int argc, char* argv[]) {
	//All parameters lists
	if ( argc < 2 ) return -1;


	//--------------Environment parameters. Outermost loops-------------------------

	//Environment types
	string environment_types[] = { "clustered", "uniform", "gaussian", "vein" };
	int num_environment_types = 4;

	//Grid sizes
	int grid_sizes[] = { 50, 100, 200, 300, 500 };
	vector<int> gridSize (grid_sizes, grid_sizes + sizeof(grid_sizes) / sizeof(int) );

	//Object percentage
	int percentage_objects[] = { 5, 20, 50, 70, 90 };
	vector<int> objectPercentage (percentage_objects, percentage_objects + sizeof(percentage_objects) / sizeof(int) );

	//Gold waste ratios
	double gold_ratios[] = { 0, 0.2, 0.25, 0.33333333, 0.5, 0.666666667, 0.75, 0.8, 1 };
	vector<double> goldWasteRatio (gold_ratios, gold_ratios + sizeof(gold_ratios) / sizeof(int) );

	//-------------Experiment parameters. Innermost loops----------------------
	//Number of robots
	int num_robots[] = { 10 };
	vector<int> numRobots (num_robots, num_robots + sizeof(num_robots) / sizeof(int) );

	//Experiments
	vector<Experiment*> experiments;
	experiments.push_back( new BeeForage() );
	experiments.push_back( new BasicForage() );

	//Forager explorer ratio
	double forager_explorer_ratio[] = { 0.1, 0.3, 0.5, 0.7, 0.9 };
	vector<double> foragerExplorerRatio (forager_explorer_ratio, forager_explorer_ratio + sizeof(forager_explorer_ratio) / sizeof(int) );

	//Gold waste division ratio
	double gold_waste_robot_division[] = {  0, 0.2, 0.25, 0.33333333, 0.5, 0.666666667, 0.75, 0.8, 1  };
	vector<double> goldWasteRobotDiv (gold_waste_robot_division, gold_waste_robot_division + sizeof(gold_waste_robot_division) / sizeof(int) );

	//-------------Values that do not change----------------------
	//Descriptors
	EXPERIMENT_DESC exp_desc;
	ENVIRONMENT_DESC env_desc;

	//Variables that do not change.
	exp_desc.max_path = 50;				//Max path
	exp_desc.samples = 30;				//Samples
	exp_desc.total_iterations = 4000;	//total iterations
	env_desc.sink_boundary = 5;			//Sink boundary
	
	//-------------Giant Experiment Loop--------------------------
	

	//Environment
//	for (int i=-1; i < num_environment_types-1; i++) {
		env_desc.type = argv[1];
		//env_desc.type = environment_types[i];
		//_beginthread( beginThread, 0 , (void *) &(i) );
		//Grid size
		for (int j=0; j < gridSize.size(); j++) {
			env_desc.grid_size = gridSize[j];

			//Percentage of objects
			for (int k=0; k < objectPercentage.size(); k++ ) {
				env_desc.num_objects = objectPercentage[k];

				//Gold to waste ratio
				for (int p=0; p < goldWasteRatio.size(); p++) {
					env_desc.ratio_gold = goldWasteRatio[p];

					//Experiment
					//Number of robots
					for (int q = 0; q < numRobots.size(); q++) {
						exp_desc.number_robots = numRobots[q];

						//Forager explorer ratio
						for (int u=0; u < foragerExplorerRatio.size(); u++) {		
							exp_desc.forager_explorer_ratio = foragerExplorerRatio[u];

							//Gold waste robot division
							for (int y=0; y < goldWasteRobotDiv.size(); y++) {		
								exp_desc.gold_waste_division_ratio = goldWasteRobotDiv[y];

								//Experiment
								for (int v = 0; v < experiments.size(); v++ ) {
									 runExperiment( experiments[v], exp_desc, env_desc);
								}
							}

						}

					}

				}
			}
		}
		
	//}
	//Bitch, please...
}

