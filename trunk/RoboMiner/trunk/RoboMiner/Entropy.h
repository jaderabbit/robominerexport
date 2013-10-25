#pragma once
#include <vector>

using namespace std;

typedef vector<vector<short int>> Grid;

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

	virtual string getName() { return "Entropy";}
	virtual bool isNext();
	virtual string getNext();
	virtual double getNextValue();
	virtual double getFinalValue();
	virtual void reset();
	virtual bool isPerIteration() {return false;}
	virtual int getId() 
	{
		return 4;
	}

private:
	//Entropy per robot
	double entropyPerRobot( Grid g, int uniquePositions );
	double entropyPerRobot( short int** g, int uniquePositions );

	//Grid per robot
	vector<Grid> robotGrids;
	//vector<short int**> robotGrids;

	Grid robotGrid;
	vector<int> uniquePosPerRobot;

	int gridSize;
	int N; //number of blocks in grid

	int time;

};

