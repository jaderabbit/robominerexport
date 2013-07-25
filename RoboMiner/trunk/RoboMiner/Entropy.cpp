#include "Entropy.h"
#include <vld.h>

Entropy::Entropy(void)
{
	time = 0;
}

Entropy::Entropy( int _gridSize, int _numRobots ) : PerformanceMeasure(),gridSize(_gridSize)
{
	//Initialize time
	time = 0;
	
	//Initialize grid per robot
	for (int i=0; i < _numRobots; i++) {
		//Initialize unique positions vector
		uniquePosPerRobot.push_back(0);

		//Initialize grid
		Grid g(_gridSize);
		for (int j=0; j < _gridSize; j++) {
			vector<short int> r(_gridSize,0);
			g[j] =r;
		}
			
		/*short int** grid = new short int*[_gridSize];
		for (int i=0; i < _gridSize; ++i) {
			grid[i] = new short int[_gridSize];
			for (int j=0; j < _gridSize; ++j) {
				grid[i][j] = 0;
			}
		}*/
		robotGrids.push_back(g);
	}

	//Calculate number of grid positions for each grid.
	N = gridSize*gridSize;


}

Entropy::~Entropy(void)
{
	/*for (unsigned int i=0; i < robotGrids.size(); i++) {
		for (int j=0; j < gridSize; j++) {
			delete [] robotGrids[i][j];
		}
		delete [] robotGrids[i];
	}*/
	robotGrids.clear();
}

void Entropy::reset() {

	robotGrids.clear();
	uniquePosPerRobot.clear();

	gridSize=0;
	N=0; //number of blocks in grid

	time=0;
}

void Entropy::takeMeasure( vector<Robot>& robots) 
{
	//Calculate measure
	for (int i=0; i < robots.size(); i ++ ) {
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
	double totalEntropy = 0;
	for (int i=0; i < uniquePosPerRobot.size(); i++) {
		totalEntropy += entropyPerRobot( robotGrids[i], uniquePosPerRobot[i]);
	}

	totalEntropy/=(1.0*robotGrids.size());
	measurement.push_back(totalEntropy);
}

double Entropy::entropyPerRobot( Grid g, int uniquePositions ) 
{
	//Calculate average
	/*double average = 0.0;
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
	return uniquenessRatio*stdDev;*/

	double total = 0.0;
	for (int j=0; j < gridSize; j++) {
		for (int k=0; k < gridSize; k++) {
			total += g[j][k];
		}
	}

	return (total > 0) ? (uniquePositions*1.0)/(total*1.0) : 0;
}


double Entropy::entropyPerRobot( short int** g, int uniquePositions ) 
{
	//Calculate average
	/*double average = 0.0;
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
	return uniquenessRatio*stdDev;*/

	double total = 0.0;
	for (int j=0; j < gridSize; j++) {
		for (int k=0; k < gridSize; k++) {
			total += g[j][k];
		}
	}

	return (total > 0) ? (uniquePositions*1.0)/(total*1.0) : 0;
}


bool Entropy::isNext() {
	return ( timer > time ) ? false : true;
}

string Entropy::getNext() {
	stringstream s1;
	if (timer < time) {
		timer++;
		return "";
	} else {		
		s1.clear();
		s1 << measurement[0];
		timer++;
		return s1.str();
	}
}
