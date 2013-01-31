#include "Experiment.h"


Experiment::Experiment(void)
{
}


Experiment::~Experiment(void)
{
}

Experiment::Experiment( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc ) : desc(_desc), env_desc(_env_desc), sampleCount(0){}

