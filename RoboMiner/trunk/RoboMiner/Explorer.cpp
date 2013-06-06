#include "Robot.h"
#include "Mine.h"
#include <iostream>

void Robot::explore() {
	switch (state) {
		case HOMING: homingStep(); break;
		case BEACON_HOMING: beaconHomingStep(); break;
		case EXPLORING: exploreStep(); break;
		case RECRUITING: recruitStep(); break;
		case UNLOADING: unloadStep(); break;
		case CHOOSE_ACTIVITY: chooseActivity(); break;
		default: cout << "Erroneous State for Exploring" << endl; break;
	}
}

double Robot::calculateDensity() {
	double total = 0;
	double occupied = 0;

	for (int i=0; i < 8; i++) {
		Coord d = dir_circle[i];
		Coord tmp_pos; tmp_pos.x = pos.x; tmp_pos.y = pos.y;

		//Run through depth of view
		for (int i=0; i < DoV; i++ ) {
			//calc new pos
			tmp_pos.x += d.x;
			tmp_pos.y += d.y;

			//Increment the total area
			total+= 1.0;

			//if not empty then increment occupied and break out of loop
			if (!isEmpty(tmp_pos)) {
				occupied+=1.0;
				break;
			}
		}
	}

	//Calculate density and return
	return occupied/total;
}

void Robot::avoidObstacle() {
	Coord option[2];
	if ( dir.x == 0 || dir.y == 0) {
		//option 1
		option[0].x = dir.x + dir.y;
		option[0].y = dir.y + dir.x;

		//option 2
		option[1].x = dir.x - dir.y;
		option[1].y = dir.y - dir.x;
	} else {
		//option 1
		option[0].x = dir.x;
		option[0].y = 0;

		//option 1
		option[1].x = 0;
		option[1].y = dir.y;
	}

	//choose to go LEFT or RIGHT
	int d =  (t.randomClosed() >= 0.5 ) ? 1 : 0;
	dir = option[d];
}

void Robot::exploreStep() {

	if (state_counter == 0 ) {
		oldSinkPos.x = pos.x;
		oldSinkPos.y = pos.y;
	}

	state_counter++;

	//walk
	randomWalkStep();

	//record step & calculate minimal thingie. 
	//Calculate the distance from sink every step.
	calculateDistanceFromSink();

	//look for item and pick up
	//Why is explorer picking stuff up?!?
	if (findItem()) {
		density = calculateDensity();	
		//Change to forager, 2013/06/06 Seeley Model 
		activity = FORAGE;
		state = HOMING;
		state_counter = 0;
		//activity_counter=0;
		activity_counter++;
		clusterLocation = previous_item_pos;
		foraged = true;
		typeForaged = division;
	} 

}

bool Robot::findItem() {
	for (int i=-1; i <= 1; i++) {
		for (int j=-1; j <= 1; j++) {
			if ( validPos(pos.x+i,pos.y+j) && mine->grid[pos.x +i][pos.y+j].type == division) {
				load_type = mine->grid[pos.x +i][pos.y+j].type;
				if (activity == FORAGE || activity == DESERTANT || activity == EXPLORE ) { //Added explorer 2013/06/06 Seeley Model 
					loaded = true;
					mine->grid[pos.x +i][pos.y+j].type = EMPTY;
					dir.x = i; dir.y = j;

					//Calculate the desirability of the location. 
					calculateDistanceFromSink();
					site_desirability= calculateLocationDesirability(distance_from_sink/max_distance_from_sink,calculateDensity());
				}
				previous_item_pos.x = pos.x +i;
				previous_item_pos.y = pos.y +j;
				return true;
			}
		}
	}
	return false;
}

void Robot::recruitStep() {
	if (state_counter == 0 ) {
		recruited=0;
	}
	if (state_counter < MAX_RECRUITMENT_REPS ) {
		//search up and down the area by the sink for waiting robots
		vector<int> waitingRobots = searchSink();

		//send message to waiting robots
		recruited += waitingRobots.size();
		for (int i=0; i < waitingRobots.size(); i++) {
			if ( (*robots)[waitingRobots[i]].state == WAITING ) {
				(*robots)[waitingRobots[i]].addRecruiterMessage(clusterLocation,oldSinkPos,division,density);
			}
		}
		state_counter++;	
	} else {
		//Done dancing. Recruit self
		addRecruiterMessage(clusterLocation,oldSinkPos,division,density);
		
		/*if ( recruited > 0 ) {
			state = EXPLORING;
			state_counter = 0;
			reset();
		} else {
			activity = FORAGE;
			state = LOCATING;
			state_counter = 0;
			reset();
		}*/
	}
}

vector<int> Robot::searchSink() {
	vector<int> waiting;

	for (int i=0; i < mine->size.y; i++) {
		for (int j=0; j < RANGE; j++) {
			if ( mine->grid[j][i] == ROBOT ) {
				waiting.push_back(mine->grid[j][i].index);
		}

		}
	}

	return waiting;
}

vector<int> Robot::radiusSearchSink() {
	vector<int> waiting;

	for (int i=0; i < RADIUS_SIZE; i++) {
		if ( pos.y-i > 0 && mine->grid[1][pos.y - i] == ROBOT ) {
			waiting.push_back(mine->grid[1][pos.y - i].index);
		}
		if ( pos.y +i < mine->grid[1].size() && mine->grid[1][pos.y + i] == ROBOT ) {
			waiting.push_back(mine->grid[1][pos.y + i].index);
		}
	}

	return waiting;
}


bool Robot::compareDesirability( double des) {
	//new desirability calculation for a single compare. Used by Explorers actually
	bool good_desirability = false;

	if (des > desirability_threshold) {
		good_desirability= true;

		//set recruitment reps
		recruitment_reps = MAX_RECRUITMENT_REPS*des;
	} else {
		good_desirability= false;
	}

	//recalculate threshold
	num_desirabilities++;
	desirability_total+= des;
	desirability_threshold = desirability_total/(1.0*num_desirabilities);


	return good_desirability;

	//return ( des1 < des2 ); //TODO: Improve. Current setup is just for prototyping.
	//it works, but not THAT well. 

}