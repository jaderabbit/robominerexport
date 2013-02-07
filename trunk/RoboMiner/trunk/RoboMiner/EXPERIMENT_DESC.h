#pragma once

struct EXPERIMENT_DESC {
	// height and width
	//TO DO: Change to size.
	int height;
	int width;

	//Robots
	int number_robots;
	float forager_explorer_ratio;
	float gold_waste_division_ratio;


	//Objects
	int number_objects;
	float gold_waste_ratio;

	//total cluster iterations
	int total_iterations;

	//Deprecated
	int total_cluster_iterations; 
	int total_forage_iterations; 
	float gold_waste_division_ratio_cluster;
	float gold_waste_division_ratio_forage;

	//Max path that robots are to be initialized with
	int max_path;
};