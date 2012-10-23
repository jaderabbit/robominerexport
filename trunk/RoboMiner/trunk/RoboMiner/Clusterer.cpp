#include "Robot.h"
#include "Mine.h"

void Robot::cluster() {
	switch (state) {
		case UNLOADED: unloadedStep(); break;
		case LOADED: loadedStep(); break;
		case CHOOSE_ACTIVITY: chooseActivity(); break;
	}
}

void Robot::loadedStep() {
	//calculate f
	calculateF();

	//randomly move
	randomWalkStep();

	//look around and pick up and put down
	unload();

	if (!loaded) {
		state = UNLOADED;
		state_counter = -1;
	}
}

void Robot::unloadedStep() {
	//calculate f
	calculateF();

	//randomly move
	randomWalkStep();

	//look around and pick up and put down
	load();

	if (loaded) {
		state = LOADED;
	}
}

void Robot::randomWalkStep() {
	if ( path_count < path_length && validMove() ) {
		makeMove();
		path_count++;
	} else {



		if ( path_count < path_length ) {
			chooseMaxPathLength();
		}
		
		//if (t.randomOpen() < 0.5)
			chooseDirection();
		//else 
		//	chooseOppositeDirection();
		path_count = 0;
	}
}

void Robot::lookAround() {
	if (loaded) {
		unload();
	} else {
		load();
	}	
}

void Robot::chooseOppositeDirection() {
	dir.x = -dir.x;
	dir.y = -dir.y;
}

void Robot::chooseDirection() {
	bool same = true;
	while (same) {
		//new possible direction
		int new_dir_x = t.random(-1,1);
		int new_dir_y = t.random(-1,1);

		//if 0 vector do nothing
		if ( new_dir_x == 0 && new_dir_y == 0) {
			same = true;
		} 
		//else set direction vector
		else if ( new_dir_x != dir.x || new_dir_y != dir.y ) {
			same = false;
			dir.x = new_dir_x;
			dir.y = new_dir_y;
		}		
	}
}

void Robot::chooseMaxPathLength() {
	path_length = t.random(1,max_path);
}

bool Robot::load() {
	double Pp = pow((f-1),4);
	double r = t.randomOpen();
	if ( r < Pp) {
		for (int i=-1; i <= 1; i++) {
			for (int j=-1; j <= 1; j++) {
				if ( validPos(pos.x+i,pos.y+j) && mine->grid[pos.x +i][pos.y+j] == division) {
					load_type = mine->grid[pos.x +i][pos.y+j].type;
					loaded = true;
					 mine->grid[pos.x +i][pos.y+j].type = EMPTY;
					return true;
				}
			}
		}
	}
		
	return false;
}

bool Robot::unload() {
	//calculate Pd
	double Pd = -pow((f -1),4)+1;
	double r = t.randomOpen();

	if ( r < Pd ) {
			//find empty spot
			for (int k=-1; k <= 1; k++) {
				for (int l=-1; l <= 1; l++) {
					if ( validPos(pos.x+k,pos.y+l) && mine->grid[pos.x +k][pos.y+l].type == EMPTY ) {
						//put down
						mine->grid[pos.x +k][pos.y+l].type = load_type;

						//change to unloaded
						loaded = false;
						load_type = 0;
						return true;
					}
				}
			}	
		}
	return false;
}

bool Robot::validPos(int x, int y) {
	if ( (x >= 0) && (x < mine->size.x)  && (y < mine->size.y) && (y >= 0)) {
		return true;
	}
	return false;
}

void Robot::calculateF() {
	double divisor = 0;
	for (int i=0; i < c; i++) {
		divisor += i*8;
	}
	divisor -= 1;
 
	if ( division == GOLD ) {
			int gold_corpses = 0;
			for (int i=-c; i <= c; i++) {
				for (int j=-c; j <= c; j++) {
					if ( validPos(pos.x+i,pos.y+j) && (mine->grid[pos.x +i][pos.y+j].type == GOLD)) {
						gold_corpses++;
					}
				}
			}
			f = ((double)gold_corpses)/divisor;
		} else if ( division == WASTE ) {
			int waste_corpses = 0;
			for (int i=-c; i <= c; i++) {
				for (int j=-c; j <= c; j++) {
					if ( validPos(pos.x+i,pos.y+j) && mine->grid[pos.x +i][pos.y+j].type == WASTE ) {
						waste_corpses++;
					}
				}
			}
			f = ((double)waste_corpses)/divisor;
		} else {
			int total_corpses = 0;
			for (int i=-c; i <= c; i++) {
				for (int j=-c; j <= c; j++) {
					if ( validPos(pos.x+i,pos.y+j) && (mine->grid[pos.x +i][pos.y+j] == GOLD || mine->grid[pos.x +i][pos.y+j] == WASTE )) {
						total_corpses++;
					}
				}
			}
			f = ((double)total_corpses)/divisor;
		}
}