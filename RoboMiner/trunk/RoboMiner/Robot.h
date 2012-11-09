#pragma once
#include "Tools.h"
#include <vector>
#include <fstream>
using namespace std;

class Mine;

class PerformanceBed;

enum { EMPTY, GOLD, WASTE, ROBOT, SINK, G_SINK, W_SINK };

enum { CLUSTER, FORAGE, EXPLORE, BASICFORAGE };


enum { HOMING, BEACON_HOMING, CHOOSE_ACTIVITY, 			//General States
	   RECRUITING, EXPLORING,					//Explorer States
	   WAITING, LOCATING, LOCAL_CLUSTER_SEARCH, LOADING, UNLOADING,	//Forager States
	   UNLOADED, LOADED						//Ant Cemetary States
	  };

struct Coord {
	int x;
	int y;

	bool operator==(const Coord &other) const {
		if ( x == other.x && y == other.y) {
			return true;
		}
		return false;
	}

	bool operator<(const Coord &other) const {
		if ( x < other.x && y < other.y) {
			return true;
		}
		return false;
	}
};

class RobotState;

class Robot
{
public:
//Methods
	//Constructors
	Robot(void);
	Robot(Mine* _mine);
	Robot( Mine* _mine, Coord _pos, Coord _dir, int _act, int _max_path, string tracker = "");
	Robot( Mine* _mine, Coord _pos, Coord _dir, int _act, int _state, int _max_path, int _div, string tracker = "");
	Robot( Mine* _mine, Coord _pos, Coord _dir, int _act, int _max_path, int _div, string tracker = "");
	~Robot(void);

	//Set activity and do activity
	void doStep();
	void setActivity( int _act);
	void setIndex (int _index ) { index = _index; }

	//Division of Labour
	void chooseActivity();

	//helper method
	int sgm(int x);
	bool isHome();

	//Tracker
	void trackMovement();

	//Method for each activity
	void explore();
	void forage();
	void cluster();

	//EXPLORING Methods
	void homingStep();
	void exploreStep();
	void recruitStep();
	void beaconHomingStep();

	//Helper EXPLORING Methods
	bool findItem();
	void avoidObstacle();
	vector<int> searchSink();
	vector<int> radiusSearchSink();

	//CLUSTERING Methods
	void unloadedStep();
	void loadedStep();
	void calculateF();
	bool load();
	bool unload();
	int opposite( int division );

	//Helper CLUSTERING Methods
	void randomWalkStep();
	void lookAround();
	void chooseDirection();
	void chooseOppositeDirection();
	void chooseMaxPathLength();
	void makeMove();
	bool validMove();
	bool validPos(int x, int y);
	bool isEmpty(int dirx, int diry );

	//FORAGING Methods
	void waitStep();
	void locateStep();
	void loadStep();
	void unloadStep();
	//void homingStep();

	//Helper FORAGING Methods
	void addRecruiterMessage( Coord location, Coord recruiterPos, int type ); 
	void localClusterSearchMovement();
	bool findItem( int searchRange );
	bool seeItem();
	void reset();
	bool walkingIntoAWall();
	bool directionYToSink();

	//Obstacle Avoidance Methods
	void calculateDistanceFromSink();
	void calculateFoV();

	//OutputMessages
	void trackerOutput();

	//getter and setters
	void setInitialPosition( int x, int y) { pos.x = x; pos.y = y;}
	void setPosition( int x, int y); 
	Coord getPosition( int x, int y) { return pos; }

	void setDir( Coord _dir ) { dir = _dir;}
	Coord getDir() { return dir; }

	int getActivity() { return activity; }

	void setState( int _state ) { state = _state; }
	int getState() { return state; }

	void setRobotState( RobotState* _state ) { robotState = _state; }
	RobotState* getRobotState() { return robotState; }

	void setMaxPath( int _max_path ) { max_path = _max_path; }

	void setStringTracker( string track_file ) { tracker = true;
		trackFile = track_file;
	}

	void setMutualRobotAwareness( vector<Robot> * _robots ) { robots = _robots; }

	void setDivision( int _div ) { division = _div;}
	int getDivision() { return division;}

	void setPerformanceBed( PerformanceBed* _performanceBed ) { performanceBed = _performanceBed; }

//Member Variables
	//Performance Measures
	PerformanceBed* performanceBed;

	//Members
	Mine* mine;		//Reference to the mine	
	Coord pos;		//position
	Coord dir;		//direction

	int activity;	//activity
	int state;		//state
	int index;

	//Load
	int load_type;
	bool loaded;	//perhaps unnecessary
	int division;

	//Random wander
	int max_path;
	int path_length;
	int path_count;

	//Repition counter
	int activity_counter;
	int state_counter;

	//Ant Cemetery Building
	double f;//Explorer States

	//initial state for each activity
	int init_states[4];
	int max_activity_counters[4];

	//distance to sink
	double max_distance_from_sink;
	double distance_from_sink;

	//Tools
	Tools t;

	//Home Vector
	Coord homeVector;
	Coord clusterLocation;
	Coord destination;

	//Recruiter Message
	Coord recruiterClusterLocation;
	Coord recruiterOriginalPos;
	Coord oldSinkPos;
	
	//variables
	const static int MAX_RECRUITMENT_REPS = 30;
	const static int MAX_LOADING_REPS = 30;
	const static int RADIUS_SIZE = 10;
	const static int MAX_PATH_DEVIATION = 4;
	const static int c = 3; //max cluster deviation
	const static int MAX_SEARCH_RANGE = 4;
	const static int MAX_STATE_COUNTER = 100;

	//if robot has a tracker, then a file is saved with its total movements
	bool tracker;
	string trackFile;
	vector<Coord> track;

	RobotState* robotState;
	vector<Robot>* robots;

	//Performance variable measures
	bool foraged;
	int typeForaged;
	bool moved;	//update in move
	void resetPerformanceMeasures();
	Coord FoV[5];
	Coord dir_circle[8];



	
};


