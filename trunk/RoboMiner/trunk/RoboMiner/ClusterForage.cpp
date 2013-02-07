#include "ClusterForage.h"
#include "ItemsForagedOverTime.h"
#include "AverageTimeInState.h"
#include "Entropy.h"

#include <sstream>

ClusterForage::ClusterForage(void)
{
}

ClusterForage::ClusterForage( EXPERIMENT_DESC _desc, ENVIRONMENT_DESC _env_desc ) : Experiment(_desc,_env_desc) {
}


ClusterForage::~ClusterForage(void)
{
}

int ClusterForage::run() {
	//CLUSTERING
	for (unsigned int i=0; i < robots.size(); i++) {
		robots[i].setActivity(CLUSTER);
	}

	while (cnt < desc.total_cluster_iterations) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}

	//FORAGE/EXPLORING
	for (unsigned int i=0; i < robots.size(); i++) {
		if ( i < desc.number_robots*desc.forager_explorer_ratio ) 
			robots[i].setActivity(FORAGE);
		else {
			robots[i].setActivity(EXPLORE);
		}
	}
	cnt=0;

	while (cnt < desc.total_forage_iterations) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}

	return true;
}

int ClusterForage::runStep() {

	//reset performance measures
	for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].resetPerformanceMeasures();
	}

	//CLUSTERING
	if ( cnt == 0 ) {
		for (unsigned int i=0; i < robots.size(); i++) {
			robots[i].setActivity(CLUSTER);
		}
	}

	if (cnt < desc.total_cluster_iterations) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}

	if (cnt == desc.total_cluster_iterations) {
		int forager_count = 0, explorer_count = 0;
		for (unsigned int i=0; i < robots.size(); i++) {
			//Set random position by sink
			Coord p = randomRobotPosition();
			robots[i].setPosition(p.x,p.y);

			//forager to explorer ratio
			if ( i <= desc.number_robots*desc.forager_explorer_ratio ) {
				robots[i].setActivity(FORAGE);
				forager_count++;
			} else {
				robots[i].setActivity(EXPLORE); //explorers need to alternate type
				explorer_count++;
			}
		}

		for (unsigned int i=0; i < forager_count; i++) {
			int division = ( i <= desc.gold_waste_division_ratio_forage*forager_count) ? GOLD : WASTE;
			robots[i].setDivision(division);
		}

		for (unsigned int i=forager_count; i < forager_count+explorer_count; i++) {
			int division = ( i-forager_count <= desc.gold_waste_division_ratio_forage*explorer_count) ? GOLD : WASTE;
			robots[i].setDivision(division);
		}

	}

	if ( cnt >= desc.total_cluster_iterations && cnt < desc.total_cluster_iterations + desc.total_forage_iterations) {
		for (unsigned int j=0; j < robots.size(); j++) {
			robots[j].doStep();
		}
		cnt++;
	}

	//Add performance measure trigger ALL
	pb->trigger();

	//Final iteration
	if ( desc.total_cluster_iterations + desc.total_forage_iterations==cnt+1) {
		pb->finalize();
	}

	return true;
}

int ClusterForage::runAllSamplesStep() {
	if ( cnt >= desc.total_cluster_iterations + desc.total_forage_iterations==cnt+1 && sampleCount < samples ) {
		//Finalize the current pb
		pb->finalize();

		//save previous results
		pbs.push_back(pb);

		//reinitialize grid
		initialize();

		//reset counter
		cnt = 0;

		//increment sample count
		sampleCount++;
	} else if ( sampleCount >= samples ) {
		//Save all experiments in the reader. 
		resultWriter.setResults(pbs,desc,env_desc);
		resultWriter.writeResultFile();
	}

	runStep();

	return true;
}

 void ClusterForage::initializePerformanceMeasures() {
	pb->attach( new ItemsForagedOverTime() );
	pb->attach( new AverageTimeInState(PM_FORAGE));
	pb->attach( new Entropy(desc.height,desc.number_robots) );
	pb->attach( new ItemsForagedOverTime() );

 }

 
void ClusterForage::initializeRobots() {

	//TODO: Allow for robots to NOT choose what items to cluster. i.e. A robot can cluster any item.

	for (int i=0; i < desc.number_robots ; i ++ ) {
		//choose position
		Coord p = randomRobotPosition();

		//generate random direction
		Coord d;
		d.x = t.random(-1,1);
		d.y = t.random(-1,1);
		
		//set position & index on board
		mine.setCell(p.x,p.y,ROBOT,i);
				
		//All need to cluster 
		int activity = CLUSTER;

		//Division
		int division = ( i < desc.gold_waste_division_ratio_cluster*desc.number_robots) ? GOLD : WASTE;

		//string stream for file name
		stringstream s;
		s << "tracker_robot_" << i << ".csv";
 
		//Construct robot
		Robot r(&mine);
		r.setInitialPosition(p.x,p.y);
		r.setDir(d);
		r.setDivision(division);
		r.setActivity(CLUSTER);
		r.setStringTracker(s.str());
		r.setMutualRobotAwareness(&robots);
		r.setIndex(i);
		r.setLambda(0.5);

		//Performance bed
		r.setPerformanceBed(pb);

		//push back robot
		robots.push_back(r);

	}

}


Coord ClusterForage::randomRobotPosition() {
	//choose position
	Coord p;

	//generate position located close to sink
	do { 
		int range = desc.number_robots/desc.width + 1;
		p.x = t.random(0,range); 
		p.y = t.random(0,desc.height-1);
	} while (!mine.isEmpty(p.x,p.y));

	return p;

}