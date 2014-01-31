#include "Robot.h"
#include "Mine.h"
#include <iostream>
#include <assert.h>
#include "RobotState.h"
#include "PerformanceBed.h"
#include <limits>

using namespace std;

void Robot::forage() {

	switch (state) {
		case HOMING: homingStep(); break;
		case BEACON_HOMING: beaconHomingStep(); break;
		case WAITING: waitStep(); break;
		case LOCATING: locateStep(); break;
		case LOCAL_CLUSTER_SEARCH: localClusterSearchMovement(); break;
		case LOADING: loadStep(); break;
		case UNLOADING: unloadStep(); break;
		case SINK_AVOIDANCE: avoidSink(); break;
		case CHOOSE_ACTIVITY: chooseActivity(); break;
		default: cout << "Erroneous state for Foraging" << endl; break;
	}
}

void Robot::waitStep() {
	if (state_counter == 0) {
		oldSinkPos.x = pos.x;
		oldSinkPos.y = pos.y;
	}
	state_counter++;

	if (state_counter > MAX_WAITING_REPS ) {
		activity = EXPLORE;
		state = EXPLORING;
		//division = original_division;
	}
}

void Robot::avoidSink() {
	if (robotInVicinity() > 1 && state_counter < 3) {

		//Calculate most clear route. 
		double min_c = numeric_limits<double>::max();
		int min_dir = -1;

		for (int i=0; i < 8; i++) {
			if ( validMove(dir_circle[i]) ){
				double c = calculateClarity(dir_circle[i]);
				if (  c < min_c ) {
					min_c = c;
					min_dir = i;
				}
			}
		}

		dir = dir_circle[min_dir];
		makeMove();
	} else {
		state_counter = 0;
		state = WAITING;
		reset();
	}

	//Increment state counter
	state_counter++;


}

void Robot::locateStep() {
	//TODO: Add obstacle avoidance to locating
	//set destination
	if (state_counter == 0 && ( activity == FORAGE || activity == DESERTANT) ) {	
		//set home as the destination
		destination.x = clusterLocation.x;
		destination.y = clusterLocation.y;
	}
	
	//follow cluster location. 
	/*do { 
		double r1 = t.randomOpen(), r2 = t.randomOpen();
		dir.x = (t.randomOpen() < 0.5) ? sgm(clusterLocation.x - pos.x) : 0;
		dir.y = (t.randomOpen() < 0.5) ? sgm(clusterLocation.y - pos.y) : 0;
	} while ( dir.x == 0 && dir.y == 0 && ((clusterLocation.x - pos.x) != 0 &&  (clusterLocation.y - pos.y != 0)));

	//assert(dir.x != 0 && dir.y != 0 );


	if (state_counter > MAX_STATE_COUNTER ) {
		clusterLocation.x = t.gaussianDistributionDiscrete(clusterLocation.x, MAX_PATH_DEVIATION);
		clusterLocation.y = t.gaussianDistributionDiscrete(clusterLocation.y, MAX_PATH_DEVIATION);
	} */

	//Choose the locating direction of the forager.
	chooseForagerLocatingDirection();

	//Increment state counter
	state_counter++;

	//Move making
	//TODO: Optimize following piece of logic which determines what action to take.

	//Make the move: Does validity check
	makeMove();

	if ( clusterLocation.x - pos.x ==0 && clusterLocation.y - pos.y ==0 ) {
		state_counter = 0;
		state = LOADING;
	} else if (seeItem()) {
		state = LOADING;
		state_counter = 0;
	} else if ( walkingIntoAWall() ) {
		state = LOADING;
		state_counter = 0;
	} 
}

bool Robot::seeItem() {
	for (int i=-1; i <= 1; i++) {
		for (int j=-1; j <= 1; j++) {
			if ( validPos(pos.x+i,pos.y+j) && mine->grid[pos.x +i][pos.y+j].type == division) {
				return true;
			}
		}
	}
	return false;
}

void Robot::loadStep() {
	//try find item
	if (findItem()) {
		//do loading
		state_counter = 0;
		state = HOMING;

		//update cluster location
		clusterLocation.x = previous_item_pos.x;
		clusterLocation.y = previous_item_pos.y;

		foraged = true;
		typeForaged = division;

		density = calculateDensity();
	} else {
		//do loading
		state_counter = 0;
		state = LOCAL_CLUSTER_SEARCH;
	}
}

void Robot::unloadStep() {
	if (loaded) {
		//unload
		loaded = false;
		mine->sink_items[load_type-1]++;

		//Old sink position
		oldSinkPos.x = pos.x;
		oldSinkPos.y = pos.y;

		//destination is set back to the location of the cluster
		destination = clusterLocation;

		if ( activity != DESERTANT) {
			//If good enough then dance
			if ( compareDesirability(site_desirability) ) {
				if (activity == FORAGE) activity_counter = 0;
				activity = EXPLORE;
				state = RECRUITING;
				state_counter = 0;
			} else {
				//If not good enough become a forager
				if (activity == EXPLORE) activity_counter = 0;
				state = LOCATING;
				activity = FORAGE;
				state_counter = 0;	
			}
		} else {
			state = LOCATING;
			state_counter = 0;	
		}
	}
	else {
		state_counter = 0;
		state = SINK_AVOIDANCE;
		reset();
		//cout << "cluster depleted, returned home empty" << endl;
	}
}

void Robot::addRecruiterMessage( Coord location, Coord recruiterPos, int type, double location_density ) {
	if ( state == RECRUITING && (division == GOLD || type == WASTE) ) {
		return;
	}
	//I BROKE SOMETHING!
	//First want to calculate desirability, then use that desirability to determine whether recruiter message is added or not
	//if ( type == division ) {	
		//If the maximum distance from the sink is 0, then this robot has never been recruited.
		if (max_distance_from_sink > 0 ) {
			//Calculate distance of new cluster location. 
			double dist_new = t.distance(pos.x,pos.y,location.x, location.y)/max_distance_from_sink; //TODO: Max distance from sink is initially zero, so that case should be catered for. 

			//Calculate distance of old cluster location
			double dist_old = t.distance(pos.x,pos.y,clusterLocation.x, clusterLocation.y)/max_distance_from_sink; //TODO: Max distance from sink is initially zero, so that case should be catered for. 
	
			//Calculate desirability
			double desire_new = calculateLocationDesirability(dist_new,location_density);
			double desire_old = calculateLocationDesirability(dist_old,density);

			//Compare desirability of new item zone to desirability of old one and choose based
			//If desirability of new one is less than that of old one then replace. 
			//if (compareDesirability( dist_new, dist_old)) {
				//Do same as in the else. 
			
				//Global Location of cluster. 
				clusterLocation = location;
				destination = location;

				//Set the destination to the cluster location
				clusterLocation.x = t.gaussianDistributionDiscrete(location.x,(1-location_density)*MAX_PATH_DEVIATION);
				clusterLocation.y = t.gaussianDistributionDiscrete(location.y,(1-location_density)*MAX_PATH_DEVIATION);
				destination = clusterLocation;

				//check if in bounds
				if (clusterLocation.x <= 0 ) clusterLocation.x = 1;
				if (clusterLocation.y >= mine->size.y ) clusterLocation.y = mine->size.y-1;

				//Use recruiter position to determine whether to take it or not

				//change state
				if (activity != FORAGE) {
					activity_counter=0;
					activity = FORAGE;
				}
				state = LOCATING;
				state_counter = 0;

				//robotState->setMinorState(LOCATING);
				//test to see how often recruited
				activity_counter++;

				//load
				division = type;
			//}
		} else {

			//change state
			if (activity != FORAGE) {
				activity_counter=0;
				activity = FORAGE;
			}

			//Global Location of cluster. 
			clusterLocation = location;
			destination = location;

			//Set the destination to the cluster location
			clusterLocation.x = t.gaussianDistributionDiscrete(location.x,MAX_PATH_DEVIATION);
			clusterLocation.y = t.gaussianDistributionDiscrete(location.y,MAX_PATH_DEVIATION);
			destination = clusterLocation;

			//check if in bounds
			if (clusterLocation.x <= 0 ) clusterLocation.x = 1;
			if (clusterLocation.y >= mine->size.y ) clusterLocation.y = mine->size.y-1;

			//Use recruiter position to determine whether to take it or not. Or not. 
			calculateDistanceFromSink();
		
			if ( distance_from_sink > max_distance_from_sink ) { max_distance_from_sink = distance_from_sink ; }
			//change state
			state = LOCATING;
			state_counter = 0;

			//robotState->setMinorState(LOCATING);
			//test to see how often recruited
			activity_counter++;

			//load
			//division = type;
		}
	//}
}

void Robot::localClusterSearchMovement() {
	if (state_counter <= MAX_SEARCH_RANGE) {
		//increment repititions
		state_counter++;

		//attempt to find item
		bool found = findItem(state_counter);

		//if item has been found and loaded
		if (found && loaded) {
			state = HOMING;
			state_counter = 0;

			//update cluster location - more accurate and closer. Utilize the direction of the item found in findItem method.
			clusterLocation.x = pos.x+dir.x;
			clusterLocation.y = pos.y+dir.y;

			foraged = true;
			typeForaged = load_type;
			
			density = calculateDensity();

		//if item has been located but not loaded
		} else if (found) {
			makeMove();
			//state_counter--;
		} else {
			max_path = 5; //local cluster search
			//choose random direction and move
			randomWalkStep();

			max_path = 50;
		}

	} else {
		//else cluster is depleted and just go home unloaded 
		if (activity == FORAGE) {
			state = HOMING;
		} else if (activity == DESERTANT) {
			state = EXPLORING;
		}
		state_counter = 0;
	}
}

bool Robot::findItem( int searchRange ) {
	for (int i=-searchRange; i <= searchRange; i++) {
		for (int j=-searchRange; j <= searchRange; j++) {
			if ( validPos(pos.x+i,pos.y+j) && mine->grid[pos.x +i][pos.y+j].type == division) {
				//if directly next to it then load
				if ( i == sgm(i) && j == sgm(j) ) {
					load_type = mine->grid[pos.x +i][pos.y+j].type;
					loaded = true;
					mine->grid[pos.x +i][pos.y+j].type = EMPTY;

					dir.x = i; dir.y = j;
					previous_item_pos.x = pos.x +i;
					previous_item_pos.y = pos.y +j;

					return true;
				} else {
					//else set direction to move towards it
					dir.x = sgm(i); dir.y = sgm(j);
				}
			}
		}
	}
	return false;
}

bool Robot::compareDesirability( double des1, double des2) {

	if (des1 < des2) {
		
	}
	return ( des1 < des2 ); //TODO: Improve. Current setup is just for prototyping.
	//it works, but not THAT well. 
}
