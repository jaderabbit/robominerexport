#include "BeeForage.h"
#include "DesertAntForage.h"
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
#include <iostream>

using namespace std;

/*
bool runExperiment( Experiment* e, EXPERIMENT_DESC exp_desc, ENVIRONMENT_DESC env_desc ) {
	//Set the experiment parameters
	e->setExperimentParam(exp_desc,env_desc);

	//Initialize
	e->initialize();

	//Run all samples
	e->runAllSamples();

	e->reset();

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
	float forager_explorer_ratio[] = { 0.1f, 0.3f, 0.5f, 0.7f, 0.9f };
	vector<float> foragerExplorerRatio (forager_explorer_ratio, forager_explorer_ratio + sizeof(forager_explorer_ratio) / sizeof(int) );

	//Gold waste division ratio
	float gold_waste_robot_division[] = {  0.0f, 0.2f, 0.25f, 0.33333333f, 0.5f, 0.666666667f, 0.75f, 0.8f, 1.0f };
	vector<float> goldWasteRobotDiv (gold_waste_robot_division, gold_waste_robot_division + sizeof(gold_waste_robot_division) / sizeof(int) );

	for (unsigned int j=0; j < gridSize.size(); j++) {
			env_desc.grid_size = gridSize[j];

			//Percentage of objects
			for (unsigned int k=0; k < objectPercentage.size(); k++ ) {
				env_desc.num_objects = objectPercentage[k];

				//Gold to waste ratio
				for (unsigned int p=0; p < goldWasteRatio.size(); p++) {
					env_desc.ratio_gold = goldWasteRatio[p];

					//Experiment
					//Number of robots
					for (unsigned int q = 0; q < numRobots.size(); q++) {
						exp_desc.number_robots = numRobots[q];

						//Forager explorer ratio
						for (unsigned int u=0; u < foragerExplorerRatio.size(); u++) {		
							exp_desc.forager_explorer_ratio = foragerExplorerRatio[u];

							//Gold waste robot division
							for (unsigned int y=0; y < goldWasteRobotDiv.size(); y++) {		
								exp_desc.gold_waste_division_ratio = goldWasteRobotDiv[y];

								//Experiment
								for (unsigned int v = 0; v < experiments.size(); v++ ) {
									 runExperiment( experiments[v], exp_desc, env_desc);
								}
							}

						}

					}

				}
			}
		}

	for (int i=0; i < experiments.size(); i++) 
	{
		delete experiments[i];
}

}*/

LPTSTR createCommandlineString( ENVIRONMENT_DESC e, EXPERIMENT_DESC d) {
	char* commandline;
	stringstream s;


	s << "RoboMinerProcess.exe" <<" " << e.ratio_gold  << " " << e.sink_boundary  << " "<< d.forager_explorer_ratio << " "<< d.max_path  << " "<< d.samples << " ";
	s << d.total_iterations;

	string value = s.str();
	const char* tmp = value.c_str();
	commandline = new char[s.str().length() + 1];
	copy(tmp, tmp + s.str().length() + 1, commandline); //why am I doing this!?
	s.str(std::string());
	s.clear();

	return commandline;
}

//#include "SQLODBC.h"


int main4(int argc, char* argv[]) {

	//Test Case 1	
	/*EXPERIMENT_DESC d;
	d.width = 50; d.height = 50;
	d.number_objects = 700;
	d.number_robots = 10;
	d.forager_explorer_ratio = 0.8f;
	d.total_iterations = 4000;
	d.gold_waste_division_ratio =1;
	d.max_path = 50;
	d.samples = 30;

	ENVIRONMENT_DESC e;
	e.grid_size = 50;
	e.num_objects = 20;
	e.ratio_gold = 0.5;
	e.type = "clustered";
	e.sink_boundary = 5;

	//Test Case 2
	EXPERIMENT_DESC d2;
	d2.width = 50; d2.height = 50;
	d2.number_objects = 700;
	d2.number_robots = 10;
	d2.forager_explorer_ratio = 0.8f;
	d2.total_iterations = 4000;
	d2.gold_waste_division_ratio =1;
	d2.max_path = 50;
	d2.samples = 30;

	ENVIRONMENT_DESC e2;
	e2.grid_size = 50;
	e2.num_objects = 50;
	e2.ratio_gold = 0.333333;
	e2.type = "vein";
	e2.sink_boundary = 5;

	runExperiment( new BeeForage(d2,e2), d2,e2);
	runExperiment( new DesertAntForage(d2,e2), d2,e2);
	runExperiment( new BasicForage(d2,e2), d2,e2);

	//All parameters lists
	/*if ( argc < 2 ) return -1;

	//-------------Values that do not change----------------------
	*/

	//Descriptors
	EXPERIMENT_DESC exp_desc;
	ENVIRONMENT_DESC env_desc;

	//Variables that do not change.
	exp_desc.max_path = 50;				//Max path
	exp_desc.samples = 30;				//Samples
	exp_desc.total_iterations = 10000;	//total iterations
	env_desc.sink_boundary = 5;			//Sink boundary
	exp_desc.forager_explorer_ratio = 0.7f; //Forager explorer ratio.

	//Environment
	//Gold waste ratios
	double gold_ratios[] = { 0, 0.2, 0.25, 0.33333333, 0.5, 0.666666667, 0.75, 0.8, 1 };
	vector<double> goldWasteRatio (gold_ratios, gold_ratios + sizeof(gold_ratios) / sizeof(double) );
	//cout << goldWasteRatio.size() << endl;

	short parts = 1; short part = 1;
	if (argc >= 3 ) {
		stringstream s1;
		s1 << argv[1];
		s1 >> parts;
		stringstream s;
		s << argv[2];
		s >> part;
	}

	unsigned int size = goldWasteRatio.size();
	unsigned int segmentSize = size/parts;
	unsigned int start = segmentSize*(part-1);
	unsigned int end = segmentSize*(part);
	if (part == parts) {
		end = size;
	}

	//Gold to waste ratio
	for (int p=start; p < end; p++) {
		env_desc.ratio_gold = goldWasteRatio[p];
		LPTSTR command = createCommandlineString(env_desc,exp_desc);
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );
		cout << command << endl;
		if (!CreateProcess(NULL,command,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi) ) {
			cout << "Error in process creation: " << command << endl; 
						}

		delete [] command;

		  // Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
	int k=0;
	cin >> k;
	return true;
}
		
