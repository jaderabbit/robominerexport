#pragma once
#include "experiment.h"
class ClusterGeneration :
	public Experiment
{
public:
	ClusterGeneration(void);
	~ClusterGeneration(void);
	ClusterGeneration(Tools &t);
	ClusterGeneration( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc  );
	virtual int initialize();
	virtual int runAllSamplesStep();
	string getEnvironmentFileName(string env_type);
	void  runAllSamples();

	//Main methods
	virtual int run();
	virtual int runStep();

	virtual void initializeRobots();
	virtual void colourMap();
	virtual void decolourMap();
	virtual string getAlgorithmName() { return "ClusterGeneration"; }
	Coord randomRobotPosition();
	void outputFile( string fname, vector<vector<Block>>& grid );

	int num_clusters;
};

