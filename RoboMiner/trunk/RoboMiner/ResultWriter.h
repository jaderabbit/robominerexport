#pragma once

#include "PerformanceBed.h"
#include "EXPERIMENT_DESC.h"
#include "ENVIRONMENT_DESC.h"

class ResultWriter
{
public:
	ResultWriter(void);
	virtual ~ResultWriter(void);
	ResultWriter( vector<PerformanceBed*> _samples, EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc );
	void setResults( vector<PerformanceBed*> _samples, EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc );

	//outputs file
	bool writeResultFile();
private:
	//Generates file path and name
	string generateFileName( EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc);
	string generateFileHeader(PerformanceBed *pb, int num_samples );

	vector<PerformanceBed*> samples;
	EXPERIMENT_DESC exp_desc;
	ENVIRONMENT_DESC env_desc;
};

