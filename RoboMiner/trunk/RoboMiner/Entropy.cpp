#include "Entropy.h"


Entropy::Entropy(void)
{
	time = 0;
}

Entropy::Entropy( int _gridSize, int _numRobots ) : gridSize(_gridSize)
{
	//Initialize time
	time = 0;

	//Initialize grid per robot
	for (int i=0; i < _numRobots; i++) {
		//Initialize unique positions vector
		uniquePosPerRobot.push_back(0);

		//Initialize grid
		Grid g;
		for (int j=0; j < _gridSize; j++) {
			vector<int> r;
			for (int k=0; k < _gridSize; k++) {
				r.push_back(0);
			}
			g.push_back(r);
		}
		robotGrids.push_back(g);
	}

	//Calculate number of grid positions for each grid.
	N = gridSize*gridSize;


}

Entropy::~Entropy(void)
{
}

void Entropy::takeMeasure( vector<Robot>& robots) 
{
	//Calculate measure
	for (int i=0; robots.size(); i ++ ) {
		if ( robotGrids[i][robots[i].pos.x][robots[i].pos.y] == 0 ) {
			uniquePosPerRobot[i]++;
		}
		robotGrids[i][robots[i].pos.x][robots[i].pos.y]++;
	}

	//Increment time counter
	time++;	
}

void Entropy::finalize() 
{
	//Calculate average spent at any position
}

double Entropy::entropyPerRobot( Grid g, int uniquePositions ) 
{
	//Calculate average
	double average = 0.0;
	for (int j=0; j < gridSize; j++) {
		for (int k=0; k < gridSize; k++) {
			average += g[j][k];
		}
	}
	average/=N;

	//Calculate Standard deviation
	double stdDev = 0.0;
	for (int j=0; j < gridSize; j++) {
		for (int k=0; k < gridSize; k++) {
			stdDev += pow(  g[j][k] - average, 2.0);
		}
	}
	stdDev = sqrt( stdDev/N);

	//Calculate Uniqueness ration
	double uniquenessRatio = uniquePositions/(1.0*N);

	//Return entropy
	return uniquenessRatio*stdDev;
}
