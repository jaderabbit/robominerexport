#pragma once

#include <vector>
#include "Robot.h" 
#include "Tools.h"
#include "EXPERIMENT_DESC.h"
#include "ENVIRONMENT_DESC.h"

using namespace std;

struct Block {
	int type;
	int index;

	bool Block::operator==(const Block &other) const {
		return ( type == other.type);
	}

	bool Block::operator==(int other) const {
		return ( type == other);
	}
};

enum {STATE_OUTPUT, INDEX_OUTPUT, ACTIVITY_OUTPUT, LOADED_OUTPUT};
enum {CLUSTERING, BASIC_FORAGE, CLUSTERED_FORAGE, CLUSTER_AND_FORAGE};

class Mine
{
public:
	Mine(void);
	~Mine(void);

//NEW METHODS
	//initialization grid
	void initializeEmptyGrid( int x, int y );

	//TODO: Move initialization to controller
	void initializeObjectsRandomly( int num_objects, int item_ratio );
	void initializeObjectsFromFile( string fileName);

	//get cells and set cells
	bool setCell( int x, int y, int value, int index = -1 );
	bool setCellIfEmpty( int x, int y, int value, int index = -1 );
	bool isValid( int x, int y);
	bool isEmpty( int x, int y ); 
	int getCell( int x, int y, int value );

	//cell index
	void setCellIndex( int x, int y, int index );
	int getCellIndex( int x, int y);


//OLD METHODS
	Mine(int x, int y, int num_rob, int num_items, int num_item_types);
	Mine(int x, int y, int num_rob, int ratio_rob, string inputFile);

	void initSink();
	void initRandomPositionRobots( int num_robots);
	void initHomeRobots( int num_robots, int ratio_rob);
	void initObjects( int num_objects, int num_item_types);

	//load world
	bool load( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc, string fileName );
	bool load(int x, int y, int num_rob, int ratio_rob, string inputFile, int algorithm);
	bool fileInput(string fname);

	//output world
	void output();
	void outputAdvanced();
	void outputAdvanced(int option);
	char determineCharacter(int option, int robot_index);
	void fileOutput(string fname);
	void robotTrackerOutput();

	//simulation methods
	void run();
	void repeatRuns( int reps);

	//experiments

	void antCemeteryAlgorithm();
	void foragingAlgorithm();
	void recruitmentAlgorithm();
	void algorithmStep();
	void recruitmentAlgorithmStep();
	void antCemetaryAlgorithmStep();
	void foragingAlgorithmStep();

	//Grid
	vector<vector<Block>> grid;
	Coord size; //not necessary if using vector

	//Robots
	vector<Robot> robots;

	//Helper methods
	Tools t;

	//Number of items in the sink
	int sink_items[2];

	//iteration counter
	int cnt;

	//algorithm type
	int alg;

	//Constants
	static const int MAX_WANDER_STEPS = 50;
	static const int MAX_ITERATIONS = 2000;

	//Sink Position
	Coord goldSinkBoundaries[2];
	Coord wasteSinkBoundaries[2];
};

