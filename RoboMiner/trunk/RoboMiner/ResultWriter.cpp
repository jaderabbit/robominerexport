#include "ResultWriter.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <vld.h>
#include "DatabaseClass.h"


ResultWriter::ResultWriter(void)
{
}

ResultWriter::~ResultWriter(void)
{
	samples.clear();
}

ResultWriter::ResultWriter( vector<PerformanceBed*> _samples, EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc, string algorithmType ) : samples(_samples), exp_desc(_exp_desc), env_desc(_env_desc) {}

void ResultWriter::setResults( vector<PerformanceBed*> _samples, EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc, string algorithmType ) {
	samples = _samples;
	exp_desc = _exp_desc;
	env_desc = _env_desc;
	algType = algorithmType;
}

void ResultWriter::reset() {
	samples.clear();
}

string ResultWriter::generateFileName( EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc) {
	stringstream name;

	//Environments
	name << "results\\" <<_env_desc.type << "\\" << _env_desc.type
		<<"_alg_" << algType<< "_size_" << _env_desc.grid_size << "_obj_" << _env_desc.num_objects << "_ratio_" << _env_desc.ratio_gold;

	//Experiment
	name << "_rob_"<< _exp_desc.number_robots << "_div_" << _exp_desc.gold_waste_division_ratio 
		<<"_iter_" << _exp_desc.total_iterations << "_mpath_" << _exp_desc.max_path << ".csv";
	
	return name.str();
}
	

string ResultWriter::generateFileHeader( PerformanceBed *pb, int num_samples ) {
	stringstream fileHeader;
	string descriptor;
	fileHeader << "i,";
	for (int i=0; i < pb->pm.size(); i++) {
		descriptor =pb->pm[i]->getName();
		
		for (int j=0; j < num_samples; j++ ) {
			fileHeader << descriptor << "(" << j << ")";
			fileHeader << ",";
		}
	}

	return fileHeader.str();
}

//outputs file
bool ResultWriter::writeResultFile() {
	//Generate name
	string fileName = generateFileName(exp_desc, env_desc);

	//Open File
	ofstream f; 
	f.open(fileName.c_str());

	//Generate header
	string header = generateFileHeader(samples[0],samples.size());

	//Output header
	f << header << endl;

	//for each iteration, for each performance measure, write out the value for each sample, for each iteration
	int num_pm = samples[0]->pm.size();
	int it = 0;
	while( samples[0]->pm[0]->isNext() ) {
		f << it << ",";
		for (int i=0; i < num_pm; i++) {
			for (int j=0; j < samples.size(); j++) {
				string tmp = samples[j]->pm[i]->getNext(); 
				f << tmp << ",";
			}

		}
		f << endl;
		it++;
	}

	//Close file	
	f.close();

	cout << "COMPLETE: " << fileName << endl;
	//Open File
	ofstream log; 
	log.open("log.txt",std::ios_base::app);
	log << "COMPLETE: " << fileName << endl;
	log.close();


	//TODO: Change function to return result of opening the file
	return true;
}

int ResultWriter::getEnvironmentTypeId( string type)
{
	switch (type[0])
	{
		case 'c' : return 1; break; //clustered
		case 'g' : return 2; break; //gaussian
		case 'u' : return 3; break; //uniform
		case 'v': return 4; break;  //vein            
	}
	return -1;
}

int ResultWriter::getAlgorithmId( string type)
{
	switch (type[0])
	{
		case 'D' : return 1; break; //Desert Ant
		case 'H' : return 2; break; //Honey Bee
		case 'B' : return 3; break; //Basic/Naive
	}
	return -1;
}


bool ResultWriter::writeResultToSql()
{
	//Database Class
	DatabaseClass* db = new DatabaseClass("Data Source=JADE-PC;Initial Catalog=Experiment;Integrated Security=True");

	db->CreateConnection();

	//Generate or get experiment
	int experimentId = db->GetExperimentID( exp_desc.gold_waste_division_ratio , exp_desc.number_robots, exp_desc.max_path);
	if (experimentId < 0)
	{
		db->AddExperiment(  exp_desc.gold_waste_division_ratio , exp_desc.number_robots, exp_desc.max_path);
		experimentId = db->GetExperimentID( exp_desc.gold_waste_division_ratio , exp_desc.number_robots, exp_desc.max_path);
	}

	//Generate or get environment
	int type = getEnvironmentTypeId(env_desc.type);
	int environmentId =  db->GetEnvironmentID( env_desc.grid_size, env_desc.num_objects, env_desc.ratio_gold, type);
	if (environmentId < 0)
	{
		db->AddEnvironment( env_desc.grid_size, env_desc.num_objects, env_desc.ratio_gold, type);
		environmentId =  db->GetEnvironmentID( env_desc.grid_size, env_desc.num_objects, env_desc.ratio_gold, type);
	}

	//Get Algorithm Id
	int algorithmId = getAlgorithmId( algType);

	//Generate all the data

	//for each iteration, for each performance measure, write out the value for each sample, for each iteration

	//For each performance measure
	int num_pm = samples[0]->pm.size();
	for (int i=0; i < num_pm; ++i)
	{
		int performanceId = samples[0]->pm[i]->getId();
		bool perIteration = samples[0]->pm[i]->isPerIteration();
		for (int j=0; j < samples.size(); j++)
		{	
			double value = samples[j]->pm[i]->getFinalValue();
			db->AddResultRow(experimentId, performanceId, algorithmId, environmentId,exp_desc.total_iterations,j,value);
			/*
			if (perIteration) {
				int it = 0;
				while( samples[j]->pm[i]->isNext() ) {
						double value = samples[j]->pm[i]->getNextValue();
						//int expId, int measId,  int algId, int envId,  int it, int sample, double value )
						db->AddResultRow(experimentId, performanceId, algorithmId, environmentId,it,j,value);
						it++;
				}
			} else{
				double value = samples[j]->pm[i]->getNextValue();
				//int expId, int measId,  int algId, int envId,  int it, int sample, double value )
				db->AddResultRow(experimentId, performanceId, algorithmId, environmentId,exp_desc.total_iterations,j,value);
			}*/
				
		}
	}

	//Close the database connection
	db->CloseConnection();
	delete db;

	//Console Logging
	string fileName = generateFileName(exp_desc, env_desc);
	cout << "COMPLETE: " << fileName << endl;

	//File Logging
	ofstream log; 
	log.open("log.txt",std::ios_base::app);
	log << "COMPLETE: " << fileName << endl;
	log.close();

	//TODO: Change function to return result of opening the file
	return true;
}