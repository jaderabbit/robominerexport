#pragma once
#include <vector>

using namespace std;

typedef vector<vector<int>> Grid;

#include "PerformanceMeasure.h"
class Entropy :
	public PerformanceMeasure
{
public:
	Entropy(void);
	Entropy( int _gridSize, int _numRobots );
	virtual ~Entropy(void);

	virtual void takeMeasure( vector<Robot>& robots);
	virtual void finalize();

private:
	//Entropy per robot
	double entropyPerRobot( Grid g, int uniquePositions );

	//Grid per robot
	vector<Grid> robotGrids;
	vector<int> uniquePosPerRobot;

	int gridSize;
	int N; //number of blocks in grid

	int time;

};

