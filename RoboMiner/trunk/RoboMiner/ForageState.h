#pragma once
#include "robotstate.h"
class ForageState :
	public RobotState
{
public:
	ForageState(void);
	ForageState( Robot* _robot);
	virtual ~ForageState(void);

	virtual void doStep();
	void forage();

	//FORAGING Methods
	void waitStep();
	void locateStep();
	void loadStep();
	void unloadStep();
	void homingStep();
	void beaconHomingStep();

	//Helper FORAGING Methods
	void addRecruiterMessage( Coord location, Coord recruiterPos, int type ); 
	void localClusterSearchMovement();
	bool findItem( int searchRange );
	bool seeItem();
	void reset();
	bool walkingIntoAWall();
	bool directionYToSink();

protected:
	const static int MAX_PATH_DEVIATION = 4;
	const static int MAX_SEARCH_RANGE = 4;
};

