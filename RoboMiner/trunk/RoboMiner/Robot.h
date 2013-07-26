#pragma once
#include "Tools.h"
#include <vector>
#include <deque>
#include <fstream>
using namespace std;

class Mine;

class PerformanceBed;

enum { EMPTY, GOLD, WASTE, ROBOT, SINK, G_SINK, W_SINK };

enum { CLUSTER, FORAGE, EXPLORE, BASICFORAGE, DESERTANT};


enum { HOMING, BEACON_HOMING, CHOOSE_ACTIVITY, 			//General States
	   RECRUITING, EXPLORING,					//Explorer States
	   WAITING, LOCATING, LOCAL_CLUSTER_SEARCH, LOADING, UNLOADING,	//Forager States
	   UNLOADED, LOADED, SINK_AVOIDANCE						//Ant Cemetary States
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
	Robot(Mine* _mine, Tools& _t);
	Robot( Mine* _mine, Coord _pos, Coord _dir, int _act, int _max_path, string tracker = "");
	Robot( Mine* _mine, Coord _pos, Coord _dir, int _act, int _state, int _max_path, int _div, string tracker = "");
	Robot( Mine* _mine, Coord _pos, Coord _dir, int _act, int _max_path, int _div, string tracker = "");
	~Robot(void);

	//Set activity and do activity
	void doStep();
	void setActivity( int _act);
	void setIndex (int _index );

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
	double calculateDensity();

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
	bool validMove( int dx, int dy );
	bool validMove( Coord p );
	bool validPos(int x, int y);
	bool isEmpty(int dirx, int diry );
	bool isEmpty(Coord p);

	//FORAGING Methods
	void waitStep();
	void locateStep();
	void loadStep();
	void unloadStep();
	void avoidSink();
	//void homingStep();

	//Helper FORAGING Methods
	void addRecruiterMessage( Coord location, Coord recruiterPos, int type, double location_density ); 
	void localClusterSearchMovement();
	bool findItem( int searchRange );
	bool seeItem();
	void reset();
	bool walkingIntoAWall();
	bool directionYToSink();
	double calculateLocationDesirability( double distance, double density );
	bool compareDesirability( double des1, double des2);

	//Obstacle Avoidance Methods
	void calculateDistanceFromSink();
	double calculateDistanceFromSink(Coord new_dir);
	double calculatePosDistanceFromSink(Coord new_pos);
	double calculateDistanceFromLocation(Coord new_dir);
	int calculateFoV();
	double calculateClarity( Coord d );
	double calculateDesirability( Coord d, double rank, double max_rank );
	void chooseForagerDirection();
	void chooseForagerLocatingDirection();
	Coord directionToSink();
	Coord directionToItem();
	bool isStuck();
	bool isEmptyVicinity();
	void adaptLambda();
	int robotInVicinity();

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

	void setMaxPath( int _max_path ) { max_path = _max_path; chooseMaxPathLength();	 }

	void setStringTracker( string track_file ) { tracker = true;
		trackFile = track_file;
	}

	void setMutualRobotAwareness( vector<Robot> * _robots ) { robots = _robots; }

	void setDivision( int _div ) { division = _div;}
	void setDivToOriginalDivision() { original_division = division;};
	int getDivision() { return division;}

	void setPerformanceBed( PerformanceBed* _performanceBed ) { performanceBed = _performanceBed; }

	int getDirectionIndex( Coord c );

	void setLambda( double _lambda ) { lambda = _lambda; }

	void setNumClusters( int _num_clusters ); 

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
	int original_division;

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
	const static int MAX_RECRUITMENT_REPS;
	const static int MAX_WAITING_REPS;
	const static int MAX_LOADING_REPS;
	const static int RADIUS_SIZE;
	const static int MAX_PATH_DEVIATION;
	const static int c; //max cluster deviation
	const static int MAX_SEARCH_RANGE;
	const static int MAX_STATE_COUNTER;
	const static int DoV;
	const static int STUCK_WINDOW_SIZE;
	const static int RANGE;
	const static int MAX_EXPLORE_STEP;

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
	Coord FoV[5]; //field of view
	double lambda;

	//Obstacle avoidance
	Coord dir_circle[8];
	deque<Coord> stuck_window;
	int one_stuck_mother_fucker;

	//Density for Explroer recruitment
	double density;

	//Position that previous item was found
	Coord previous_item_pos;
	int recruited;

	//Desirability threshold
	bool compareDesirability( double des);
	double desirability_threshold;
	double desirability_total;
	int num_desirabilities;
	double site_desirability;

	//Recruitment reps
	int recruitment_reps; //length of dance. Scaled linearly based on desirability.  

	//Cluster generation. 
	vector<double> lambdas;
	vector<deque<int>> T;
	const static int T_size;
	double pickup_prob_const;
	double drop_prob_const;
	void randomWalkStepCluster();
	void makeUnsafeMove();
	double calculateDensityType(int);

	int num_clusters;

};


