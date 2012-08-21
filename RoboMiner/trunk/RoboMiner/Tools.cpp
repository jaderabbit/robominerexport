#include "Tools.h"
#include <time.h>

Tools::Tools(void)
{
	unsigned long init[4]={0x123, 0x234, 0x345, 0x457}, length=4;
    mt = new MersenneTwister();
    mt->init_by_array(init, length);
	mt->init_genrand(time(0));
	srand(time(0));
}


Tools::~Tools(void)
{
	//delete mt;
}
