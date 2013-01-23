#include "ResultWriter.h"
#include <sstream>

ResultWriter::ResultWriter(void)
{
}


ResultWriter::~ResultWriter(void)
{
}

ResultWriter::ResultWriter( vector<PerformanceBed*> _samples, EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc ) : samples(_samples), exp_desc(_exp_desc), env_desc(_env_desc) {}

void ResultWriter::setResults( vector<PerformanceBed*> _samples, EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc ) {
	samples = _samples;
	exp_desc = _exp_desc;
	env_desc = _env_desc;
}

string ResultWriter::generateFileName( EXPERIMENT_DESC _exp_desc, ENVIRONMENT_DESC _env_desc) {
	stringstream name;

	//Environments
	name << _env_desc.type << "\\" << _env_desc.type
		<< "_size_" << _env_desc.grid_size << "_obj_" << _env_desc.num_objects << "_ratio_" << _env_desc.ratio_gold;

	//Experiment
	name << "_rob_"<< _exp_desc.number_objects << "_div_" << _exp_desc.gold_waste_division_ratio 
		<<"_iter_" << _exp_desc.total_iterations << "_mpath_" << _exp_desc.total_iterations << ".csv";
	
	return name.str();
}
	

string ResultWriter::generateFileHeader( PerformanceBed *pb, int num_samples ) {
	stringstream fileHeader;
	string descriptor;
	for (int i=0; i < pb->pm.size(); i++) {
		descriptor =pb->pm[i]->getName();
		
		for (int j=0; j < num_samples; j++ ) {
			fileHeader << descriptor << "(" << j << ")";
			fileHeader << ",";
		}
	}
	return fileHeader.str();
}