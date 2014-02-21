//#include <vld.h>
#include "BeeForage.h"
#include "BasicForageGoldOnly.h"
#include "BasicForage.h"
#include "DesertAntForage.h"
#include "Experiment.h"
#include <iostream>
#include <sstream>
#include "Tools.h"
#include "ClusterGeneration.h"
//#include "MemLeakDetect.h"
//#include "DatabaseClass.h"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
// Detect Memory Leaks

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

int getAlgorithmId( string type)
{
	switch (type[0])
	{
		case 'D' : return 1; break; //Desert Ant
		case 'H' : return 2; break; //Honey Bee
		case 'B' : return 3; break; //Basic/Naive
	}
	return -1;
}

int getEnvironmentId( string type)
{
	switch (type[0])
	{
		case 'c' : return 1; break; //Clustered
		case 'g' : return 2; break; //Gaussian
		case 'u' : return 3; break; //Uniform
		case 'v' : return 4; break; //Vein
	}
	return -1;
}

bool runExperiment( Experiment* e, EXPERIMENT_DESC d, ENVIRONMENT_DESC en, bool runCheck ) 
{

	if (runCheck)
	{
		/*DatabaseClass* db = new DatabaseClass("Data Source=JADE-PC;Initial Catalog=Experiment;Integrated Security=True");
		db->CreateConnection();
		bool exists = db->CheckResultExists(getAlgorithmId(e->getAlgorithmName()),d.gold_waste_division_ratio,d.number_robots,en.grid_size,en.num_objects,en.ratio_gold,getEnvironmentId(en.type));
		db->CloseConnection();
		delete db;
		if (exists) 
			return 1;*/
	}

	//Set the experiment parameters
	e->setExperimentParam(d,en);

	//Initialize
	e->initialize();

	//Run all samples
	e->runAllSamples();

	return true;
}

int main4(int argc, char* argv[]) 
{
	//int k;
	//cin >> k;	*/
	Tools t;

	//Pull data from environment variables
	ENVIRONMENT_DESC e;
	e.ratio_gold = convertDouble(argv[1]);
	e.sink_boundary = convertInt(argv[2]);

	EXPERIMENT_DESC d;
	d.forager_explorer_ratio = convertFloat(argv[3]);
	d.max_path = convertInt(argv[4]);
	d.samples = convertInt( argv[5] );
	d.total_iterations = convertInt( argv[6] );

	d.max_path = 30;
	d.total_iterations = 10000;

	cout << argv[1] << endl;
	cout << argv[2] << endl;
	cout << argv[3] << endl;
	cout << argv[4] << endl;
	cout << argv[5] << endl;
	cout << argv[6] << endl;

	//Setup all the parameters to be run.
	//Environment types

	string environment_types[] = {"vein","gaussian", "clustered","uniform"};
	int num_environment_types = 4;


	//Grid sizes
	//int grid_sizes[] = {50, 100, 200, 300,500};
	int grid_sizes[] = {50, 100, 200};
	//int grid_sizes[] = {300, 500};
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
	vector<double> goldWasteRobotDiv (gold_waste_robot_division, gold_waste_robot_division + sizeof(gold_waste_robot_division) / sizeof(double) );

	//Environment Types
		//Grid size
	for (int j=0; j < gridSize.size(); j++) {
		e.grid_size = gridSize[j];

		for (int i=0; i < num_environment_types; i++) {
			e.type = environment_types[i];

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
						//experiments.push_back( new BeeForage(t) );
						//experiments.push_back( new BasicForage(t) );
						experiments.push_back (new DesertAntForage(t) );
						//experiments.push_back ( new ClusterGeneration(t));

						for (int v = 0; v < experiments.size(); v++ ) {
							runExperiment( experiments[v], d, e, false);
							delete experiments[v];
						}
					}
				}
			}
		}
	}
	return true;
	
}
