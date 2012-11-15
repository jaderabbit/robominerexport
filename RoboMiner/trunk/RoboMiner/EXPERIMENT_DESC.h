#pragma once

struct EXPERIMENT_DESC {
	// height and width
	int height;
	int width;

	//Robots
	int number_robots;
	float forager_explorer_ratio;
	float gold_waste_division_ratio;
	float gold_waste_division_ratio_cluster;
	float gold_waste_division_ratio_forage;

	//Objects
	int number_objects;
	float gold_waste_ratio;

	//total cluster iterations
	int total_cluster_iterations;
	int total_iterations;
	int total_forage_iterations;

	int max_path;
};