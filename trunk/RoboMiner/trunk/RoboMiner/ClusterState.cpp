#include "ClusterState.h"
#include "Mine.h"

ClusterState::ClusterState(void)
{
}


ClusterState::~ClusterState(void)
{
}

ClusterState::ClusterState(Robot* _robot) : RobotState(_robot){
	setMinorState(UNLOADED);
}

void ClusterState::doStep() {
	cluster();
}

void ClusterState::cluster() {
	switch (state) {
		case UNLOADED: unloadedStep(); break;
		case LOADED: loadedStep(); break;
		case CHOOSE_ACTIVITY: robot->chooseActivity(); break;
	}
}

void ClusterState::loadedStep() {
	//calculate f
	calculateF();

	//randomly move
	randomWalkStep();

	//look around and pick up and put down
	unload();

	if (!robot->loaded) {
		state = UNLOADED;
		robot->state_counter = -1;
	}
}

void ClusterState::unloadedStep() {
	//calculate f
	calculateF();

	//randomly move
	randomWalkStep();

	//look around and pick up and put down
	load();

	if (robot->loaded) {
		state = LOADED;
	}
}

void ClusterState::randomWalkStep() {
	if ( robot->path_count < robot->path_length && robot->validMove() ) {
		robot->makeMove();
		robot->path_count++;
	} else {



		if ( robot->path_count < robot->path_length ) {
			chooseMaxPathLength();
		}
		
		//if (t.randomOpen() < 0.5)
			chooseDirection();
		//else 
		//	chooseOppositeDirection();
		robot->path_count = 0;
	}
}

void ClusterState::lookAround() {
	if (robot->loaded) {
		unload();
	} else {
		load();
	}	
}

void ClusterState::chooseOppositeDirection() {
	robot->dir.x = -robot->dir.x;
	robot->dir.y = -robot->dir.y;
}

void ClusterState::chooseDirection() {
	bool same = true;
	while (same) {
		//new possible direction
		int new_dir_x = robot->t.random(-1,1);
		int new_dir_y = robot->t.random(-1,1);

		//if 0 vector do nothing
		if ( new_dir_x == 0 && new_dir_y == 0) {
			same = true;
		} 
		//else set direction vector
		else if ( new_dir_x != robot->dir.x || new_dir_y != robot->dir.y ) {
			same = false;
			robot->dir.x = new_dir_x;
			robot->dir.y = new_dir_y;
		}		
	}
}

void ClusterState::chooseMaxPathLength() {
	robot->path_length = robot->t.random(1,robot->max_path);
}

bool ClusterState::load() {
	double Pp = pow((robot->f-1),4);
	double r = robot->t.randomOpen();
	if ( r < Pp) {
		for (int i=-1; i <= 1; i++) {
			for (int j=-1; j <= 1; j++) {
				if ( validPos(robot->pos.x+i,robot->pos.y+j) && robot->mine->grid[robot->pos.x +i][robot->pos.y+j] == robot->division) {
					robot->load_type = robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type;
					robot->loaded = true;
					robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type = EMPTY;
					return true;
				}
			}
		}
	}
		
	return false;
}

bool ClusterState::unload() {
	//calculate Pd
	double Pd = -pow((robot->f -1),4)+1;
	double r = robot->t.randomOpen();

	if ( r < Pd ) {
			//find empty spot
			for (int k=-1; k <= 1; k++) {
				for (int l=-1; l <= 1; l++) {
					if ( validPos(robot->pos.x+k,robot->pos.y+l) && robot->mine->grid[robot->pos.x +k][robot->pos.y+l].type == EMPTY ) {
						//put down
						robot->mine->grid[robot->pos.x +k][robot->pos.y+l].type = robot->load_type;

						//change to unloaded
						robot->loaded = false;
						robot->load_type = 0;
						return true;
					}
				}
			}	
		}
	return false;
}

bool ClusterState::validPos(int x, int y) {
	if ( (x >= 0) && (x < robot->mine->size.x)  && (y < robot->mine->size.y) && (y >= 0)) {
		return true;
	}
	return false;
}

void ClusterState::calculateF() {
	if ( robot->division == GOLD ) {
			int gold_corpses = 0;
			for (int i=-1; i <= 1; i++) {
				for (int j=-1; j <= 1; j++) {
					if ( validPos(robot->pos.x+i,robot->pos.y+j) && (robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == GOLD)) {
						gold_corpses++;
					}
				}
			}
			robot->f = ((double)gold_corpses)/7.0;
		} else if ( robot->division == WASTE ) {
			int waste_corpses = 0;
			for (int i=-1; i <= 1; i++) {
				for (int j=-1; j <= 1; j++) {
					if ( validPos(robot->pos.x+i,robot->pos.y+j) && robot->mine->grid[robot->pos.x +i][robot->pos.y+j].type == WASTE ) {
						waste_corpses++;
					}
				}
			}
			robot->f = ((double)waste_corpses)/7.0;
		} else {
			int total_corpses = 0;
			for (int i=-1; i <= 1; i++) {
				for (int j=-1; j <= 1; j++) {
					if ( validPos(robot->pos.x+i,robot->pos.y+j) && (robot->mine->grid[robot->pos.x +i][robot->pos.y+j] == GOLD || robot->mine->grid[robot->pos.x +i][robot->pos.y+j] == WASTE )) {
						total_corpses++;
					}
				}
			}
			robot->f = ((double)total_corpses)/7.0;
		}
}
