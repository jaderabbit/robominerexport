#include "BeeForage.h"
#include "BasicForageGoldOnly.h"
#include "BasicForage.h"
#include "DesertAntForage.h"
#include "Experiment.h"
#include <iostream>
#include <sstream>

using namespace std;

enum { BasicForageExperiment, BeeForageExperiment, DesertAntForageExperiment };

int convertInt( char* text ) 
{
	int num;
	istringstream convert( text );
	convert >> num;
	return num;
}

double convertDouble( char* text ) 
{
	double num;
	istringstream convert( text );
	convert >> num;
	return num;
}

float convertFloat( char* text) {
	float num;
	istringstream convert( text );
	convert >> num;
	return num;
}

/*bool runExperiment( Experiment* e, EXPERIMENT_DESC d, ENVIRONMENT_DESC en ) {
	//Set the experiment parameters
	e->setExperimentParam(d,en);

	//Initialize
	e->initialize();

	//Run all samples
	e->runAllSamples();

	return true;
}

int _tmain(int argc, char* argv[]) 
{
	//Test
	ENVIRONMENT_DESC e;
	e.ratio_gold = 0.5;
	e.sink_boundary = 5;

	EXPERIMENT_DESC d;
	d.forager_explorer_ratio = 0.7;
	d.max_path = 50;
	d.samples = 30;
	d.total_iterations = 5000;

	/*
	//Pull data from environment variables
	ENVIRONMENT_DESC e;
	e.ratio_gold = convertDouble(argv[1]);
	e.sink_boundary = convertInt(argv[2]);

	EXPERIMENT_DESC d;
	d.forager_explorer_ratio = convertFloat(argv[3]);
	d.max_path = convertInt(argv[4]);
	d.samples = convertInt( argv[5] );
	d.total_iterations = convertInt( argv[6] );*/

	//Setup all the parameters to be run.

	//Environment types
/*	string environment_types[] = { "clustered", "uniform", "gaussian", "vein" };
	int num_environment_types = 4;

	//Grid sizes
	int grid_sizes[] = { 50, 100, 200, 300, 500 };
	vector<int> gridSize (grid_sizes, grid_sizes + sizeof(grid_sizes) / sizeof(int) );

	//Object percentage
	int percentage_objects[] = { 5, 20, 50, 70, 90 };
	vector<int> objectPercentage (percentage_objects, percentage_objects + sizeof(percentage_objects) / sizeof(int) );

	//-------------Experiment parameters. Innermost loops----------------------
	//Number of robots
	int num_robots[] = {10, 30, 50, 70, 100 };
	vector<int> numRobots (num_robots, num_robots + sizeof(num_robots) / sizeof(int) );

	//Experiments

	//Gold waste division ratio
	double gold_waste_robot_division[] = {  0, 0.2, 0.25, 0.33333333, 0.5, 0.666666667, 0.75, 0.8, 1  };
	vector<double> goldWasteRobotDiv (gold_waste_robot_division, gold_waste_robot_division + sizeof(gold_waste_robot_division) / sizeof(int) );

	//Environment Types
	for (int i=0; i < num_environment_types; i++) {
		e.type = environment_types[i];

		//Grid size
		for (int j=0; j < gridSize.size(); j++) {
			e.grid_size = gridSize[j];

			//Percentage of objects
			for (int k=0; k < objectPercentage.size(); k++ ) {
				e.num_objects = objectPercentage[k];

				//Experiment
				//Number of robots
				for (int q = 0; q < numRobots.size(); q++) {
					d.number_robots = numRobots[q];

					//Gold waste robot division
					for (int y=0; y < goldWasteRobotDiv.size(); y++) {		
						d.gold_waste_division_ratio = goldWasteRobotDiv[y];

						//Experiment
					    vector<Experiment*> experiments;
						experiments.push_back( new BeeForage() );
						experiments.push_back( new BasicForage() );
						experiments.push_back (new DesertAntForage() );

						for (int v = 0; v < experiments.size(); v++ ) {
								//runExperiment( experiments[v], d, e);
							cout << "Processes going yep" << endl;
						}
					}

				}
			}
		}
		
	}

	return true;

}*/

