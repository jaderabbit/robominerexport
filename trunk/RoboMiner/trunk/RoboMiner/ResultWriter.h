#pragma once

#include "PerformanceBed.h"
#include "EXPERIMENT_DESC.h"
#include "ENVIRONMENT_DESC.h"

class ResultWriter
{
public:
	ResultWriter(void);
	virtual ~ResultWriter(void);
	ResultWriter( vector<PerformanceBed*> _samples, EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc, string algorithmType );
	void setResults( vector<PerformanceBed*> _samples, EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc, string algorithmType );

	//Generates file path and name
	string generateFileName( EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc);
	string generateSummaryResultFileName(EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc);
	string generateSummaryFileHeader();
	void writeSummaryResultFile();
	string getEnvironmentResult(ENVIRONMENT_DESC _env_desc);
	string getExperimentResult(EXPERIMENT_DESC _exp_desc);

	void reset();
	//outputs file

	bool writeResultFile();
	bool writeResultToSql();

	int getEnvironmentTypeId(string type);
	int getAlgorithmId(string type);

private:
	string generateFileHeader(PerformanceBed *pb, int num_samples );
	vector<PerformanceBed*> samples;
	EXPERIMENT_DESC exp_desc;
	ENVIRONMENT_DESC env_desc;
	string algType;

};

