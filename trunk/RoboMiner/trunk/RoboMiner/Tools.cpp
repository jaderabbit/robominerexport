#include "Tools.h"
#include <time.h>

Tools::Tools(void)
{
	//Old Mersenne
	unsigned long init[4]={0x123, 0x234, 0x345, 0x457}, length=4;
    mt = new MersenneTwister();
    mt->init_by_array(init, length);
	mt->init_genrand(time(0));

	//New Mersenne
	irand.seed(init,length);
	drand.seed(init,length);
	orand.seed(init,length);
	crand.seed(init,length);


	//Built in Random
	srand(time(0));
}


Tools::~Tools(void)
{
	//delete mt;
}
