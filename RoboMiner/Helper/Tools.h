#pragma once

#include "RoboMiner.h"

#include "mt.h"
#include "mtrand.h"
#include <limits>
#include <random>
#include <cmath>
#include "Coord.h"
#include <algorithm>

using namespace std;

#define round(x) int(x + 0.5)

class Tools
{
public:
	Tools(void);
	~Tools(void);

	double distance ( int x0, int x1, int y0, int y1 ) {
		return sqrt( pow(x0 - x1,2.0) + pow(y0 - y1, 2.0) );
	}

	int random(int low, int high) {
		//int t = mt->genrand_int31();
		//int t = rand();
		int t = irand();
		return abs( t % (high - low + 1) ) + low;
	}

	double randomClosed() {
		//return mt->genrand_real1();
		//return rand()/RAND_MAX;
		return crand();
	}

	double randomOpen() {
		//return mt->genrand_real3();
		//return rand()/RAND_MAX;
		return orand();
	}

	double gaussianDistribution( double location, double scale ) {
		double q, u, v, x, y;

		do { 
			u = randomClosed();
			v = randomClosed();

			if (u <= 0.0 || v <= 0.0) {
                u = 1.0;
                v = 1.0;
            }
            v = 1.7156 * (v - 0.5);

            /*  Evaluate the quadratic form */
            x = u - 0.449871;
            y = abs(v) + 0.386595;
            q = x * x + y * (0.19600 * y - 0.25472 * x);

            /* Accept P if inside inner ellipse */
            if (q < 0.27597) {
                break;
            }

	    /*  Reject P if outside outer ellipse, or outside acceptance region */
        } while ((q > 0.27846) || (v * v > -4.0 * log(u) * u * u));

		return max(( location + scale*v/u),0.0);
	}

	int gaussianDistributionDiscrete( double location, double scale ) {
		int rand = 0;
		do {
			rand = gaussianDistributionDiscreteSampler(location, scale) ;
		} while ( abs(rand - location) > scale );

		return rand;
	}

	int gaussianDistributionDiscreteSampler( double location, double scale ) {
		double q, u, v, x, y;

		do { 
			u = randomClosed();
			v = randomClosed();

			if (u <= 0.0 || v <= 0.0) {
                u = 1.0;
                v = 1.0;
            }
            v = 1.7156 * (v - 0.5);

            /*  Evaluate the quadratic form */
            x = u - 0.449871;
            y = abs(v) + 0.386595;
            q = x * x + y * (0.19600 * y - 0.25472 * x);

            /* Accept P if inside inner ellipse */
            if (q < 0.27597) {
                break;
            }

	    /*  Reject P if outside outer ellipse, or outside acceptance region */
        } while ((q > 0.27846) || (v * v > -4.0 * log(u) * u * u));

		return round( location + scale*v/u);
	}

	void sortConcurrent( double num[], Coord a2[], int size );


	//Random Object
	MersenneTwister *mt;

	MTRand_int32 irand; // 32-bit int generator
	MTRand drand; // double in [0, 1) generator
	MTRand_open orand; // (0,1)
	MTRand_closed crand; // [0,1]
};

