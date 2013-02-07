#include "Tools.h"
#include <time.h>
#include "Robot.h"

Tools::Tools(void)
{
	//Old Mersenne
	unsigned long init[4]={0x123, 0x234, 0x345, 0x457}, length=4;
    mt = new MersenneTwister();
    mt->init_by_array(init, length);
	mt->init_genrand(time(0));

	//New Mersenne
	drand.seed(time(0));
	orand.seed(time(0));
	crand.seed(time(0));
	irand.seed(time(0));


	//Built in Random
	srand(time(0));
}


Tools::~Tools(void)
{
	//delete mt;
}

void Tools::sortConcurrent( double num[], Coord a2[], int size ){
	int i, j, flag = 1;    // set flag to 1 to start first pass
	int temp;             // holding variable
	int numLength = size;
	Coord t;
	for (i = 1; (i <= numLength) && flag; i++)
	{
		flag = 0;
		for (j=0; j < (numLength -1); j++)
		{
			if (num[j+1] > num[j])      // ascending order simply changes to <
			{ 
					// swap elements
                temp = num[j];             
                num[j] = num[j+1];
                num[j+1] = temp;

				t = a2[j];
				a2[j] = a2[j+1];
				a2[j+1] = t;

				//swap second set of elements
                flag = 1;               // indicates that a swap occurred.
            }
        }
	}

	return;
}
