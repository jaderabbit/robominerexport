#include "Experiment.h"


Experiment::Experiment(void)
{
}


Experiment::~Experiment(void)
{
}

Experiment::Experiment( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc ) : desc(_desc), env_desc(_env_desc), sampleCount(0), cnt(0), samples(_desc.samples){}

void Experiment::setExperimentParam( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc) {
	//Clean grid and robots
	cleanup();

	//Set the new parameters
	desc = _desc;
	env_desc = _env_desc;
	sampleCount = 0;
	cnt = 0;
	samples = desc.samples;
}

int Experiment::initialize() {
	initializeGrid(); //TODO: Have a "createEnvironmentFileName" method for a given environment descriptor.
	initializeSink();
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

int Experiment::cleanup() {
	//TODO: Certify that all clean up issues have been addressed. 
	robots.clear();
	mine.grid.clear();
	
	//delete performance bed
	if (pb==0) delete pb;

	return true;
}

void Experiment::initializeGrid() {
	//Initialize the empty grid
	mine.initializeEmptyGrid(desc.width,desc.height);
	mine.load(desc, env_desc,getEnvironmentFileName());
}

void Experiment::initializeSink() {
	//init sink
	mine.initSink();
}

int Experiment::runAllSamplesStep() {
	int k=0;
	if (  getTotalIterations() ==cnt && sampleCount < samples ) {
		//Finalize
		pb->finalize();

		//save previous results
		pbs.push_back(pb);
	
		//increment sample count
		sampleCount++;

		//reinitialize grid
		cleanup();
		initialize();

		//reset counter
		cnt = 0;

	}
		
	if ( sampleCount == samples ) {
		//Save all experiments in the reader. 
		resultWriter.setResults(pbs,desc,env_desc);
		resultWriter.writeResultFile();

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

void  Experiment::runAllSamples() {
	for (int i=0; i < samples; i++) {
		for (int j=0; j < getTotalIterations(); j++) {
			//perform the step
			runStep();
		}

		//For each samples
		//Finalize
		pb->finalize();

		//save previous results
		pbs.push_back(pb);
	
		//increment sample count
		sampleCount++;

		//reinitialize grid
		cleanup();
		initialize();

		//reset counter
		cnt = 0;

	}

	//Save all experiments in the reader. 
	resultWriter.setResults(pbs,desc,env_desc);
	resultWriter.writeResultFile();

	//End of experiment
}

int Experiment::getTotalIterations() {
	return desc.total_iterations;
}

string Experiment::getEnvironmentFileName() {
	stringstream fileName;
	fileName << "environments\\" <<env_desc.type << "\\";
	fileName << env_desc.type;
	fileName << "_size_" << env_desc.grid_size;
	fileName << "_obj_" << env_desc.num_objects;
	fileName << "_ratio_" << env_desc.ratio_gold;
	fileName << "_sim_" << sampleCount << ".txt";
	return fileName.str();
}
