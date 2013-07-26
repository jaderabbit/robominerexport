#include "ClusterGeneration.h"
#include <iostream>
#include <fstream>

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

void ClusterGeneration::outputFile( string fname, vector<vector<Block>>& grid ) {
	ofstream f; 
	f.open(fname);

	int gold = 0;
	int waste = 0;
	int robots = 0;
	int sink_num = 0;
	for (unsigned int j=0; j < grid.size(); j++) {
		for (unsigned int i=0; i < grid.size(); i++) {
			char a;
			switch( grid[i][j].type ) {
				case 0: { a = '.'; break;}
				case 1: {a = 'G'; break;}
				case 2: {a = 'W'; break;}
				case 3: {a = 'R'; break;}
				case 4: {a = 'S'; break;}
				case 5: {a = 'g'; break;}
				case 6: {a = 'w'; break;}
				default: cout << "invalid type" << endl;
			}
			f << a << " ";
		}
		f << endl;
	}
	
	f << endl;
	f.close();
}

int  ClusterGeneration::runAllSamplesStep() {
	int k=0;
	if (  getTotalIterations() ==cnt && sampleCount < samples ) {
		//No result setup
		decolourMap();
		string new_file = getEnvironmentFileName("clustered");
		outputFile(new_file,mine.grid);

		//increment sample count
		sampleCount++;

		//reinitialize grid
		cleanup();
		initialize();

		//reset counter
		cnt = 0;

	}
		
	if ( sampleCount == samples ) {
		//Need to write environment out. 
		//Decolour

		//Save all experiments in the reader. 
		//resultWriter.setResults(pbs,desc,env_desc,getAlgorithmName());
		//resultWriter.writeResultFile();

		//To ensure termination. 
		//Yes, but
		sampleCount++;
		sampleCount=0;

		return true;
	} else
		runStep();
	//This should not be run of sampleCount == samples
	

	//Return false to show that the experiment is not yet finished
	return false;
}


void  ClusterGeneration::runAllSamples() {
	for (int i=0; i < samples; i++) {
		for (int j=0; j < getTotalIterations(); j++) {
			//perform the step
			runStep();
		}

		//Decolour the map
		decolourMap();

		//Write the cluster file. 
		string new_file = getEnvironmentFileName("clustered");
		outputFile(new_file,mine.grid);
	
		//increment sample count
		sampleCount++;

		//reinitialize grid
		cleanup();
		initialize();

		//reset counter
		cnt = 0;

	}

	//End of experiment
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
		r.setNumClusters(num_clusters);

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

void ClusterGeneration::colourMap() {
	int ratio_gold = env_desc.ratio_gold;
	int num_gold = env_desc.num_objects*ratio_gold;
	int num_waste = env_desc.num_objects - num_gold;
	int randomClusterSize = 10;
	num_clusters = env_desc.grid_size/randomClusterSize;
	int num_gold_clusters = env_desc.ratio_gold*num_clusters;
	int num_waste_clusters = num_clusters - num_gold_clusters;
	
	//Assign new values to the graph. 
	for (int i=0; i < env_desc.grid_size; i++) {
		for (int j=0; j < env_desc.grid_size; j++) {
			int cell_type =	mine.grid[i][j].type;
			if (cell_type == GOLD) {
				mine.grid[i][j].type = t.randomNoHigh(7 , 7 +num_gold_clusters);
			} else if (cell_type == WASTE) {
				mine.grid[i][j].type = t.randomNoHigh(7 + num_gold_clusters, 7 + num_gold_clusters+ num_waste_clusters);
			}
		}
	}


}

string ClusterGeneration::getEnvironmentFileName(string env_type) {
	stringstream fileName;
	fileName << "environments\\" <<env_type << "\\";
	fileName << env_type;
	fileName << "_size_" << env_desc.grid_size;
	fileName << "_obj_" << env_desc.num_objects;
	fileName << "_ratio_" << env_desc.ratio_gold;
	fileName << "_sim_" << sampleCount << ".txt";
	return fileName.str();
}

void ClusterGeneration::decolourMap()
{
	int ratio_gold = env_desc.ratio_gold;
	int num_gold = env_desc.num_objects*ratio_gold;
	int num_waste = env_desc.num_objects - num_gold;
	int randomClusterSize = 10;
	int num_clusters = env_desc.grid_size/randomClusterSize;
	int num_gold_clusters = env_desc.ratio_gold*num_clusters;
	int num_waste_clusters = num_clusters - num_gold_clusters;
	
	//Assign new values to the graph. 
	for (int i=0; i < env_desc.grid_size; i++) {
		for (int j=0; j < env_desc.grid_size; j++) {
			int cell_type =	mine.grid[i][j].type -7;
			if (0 <= cell_type && cell_type <   num_gold_clusters) {
				mine.grid[i][j].type = GOLD;
			} else if (num_gold_clusters <= cell_type && cell_type < num_gold_clusters + num_waste_clusters ) {
				mine.grid[i][j].type = WASTE;
			}
		}
	}
}

int ClusterGeneration::initialize() {
	initializeGrid(); //TODO: Have a "createEnvironmentFileName" method for a given environment descriptor.
	
	colourMap();

	number_robots = ceil((desc.number_robots*1.0/100.0)*env_desc.grid_size);
	initializeRobots();

	//Initialize Performance measures
	pb = new PerformanceBed(robots);

	for (int i=0; i < robots.size(); i++) {
		robots[i].setPerformanceBed(pb);
	}

	//No performance measures

	cnt = 0;
	return true;
}
