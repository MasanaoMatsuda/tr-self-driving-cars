#ifndef MALCOV_LOCALIZATION_C
#define MALCOV_LOCALIZATION_C

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "Statistics.h"

class MarkovLocalizor
{
	int map_size;
	std::vector<float> landmarks;
	std::vector<float> priors;
	std::vector<float> posteriors;
	float pos_stdev;
	float control_stdev;
	float observation_stdev;
	Statistics statistics;
public:
	MarkovLocalizor(int map_size, std::vector<float> landmarks, float pos_stdev, float control_stdev, float observation_stdev);
	void estimate_posteriors(float movement_per_timestep, std::vector<float> observations);
	void print_posterior();
private:
	std::vector<float> initialize_priors();
	std::vector<float> initialize_posteriors();
	float motion_model(float pseudo_pos, float movement);
	float observation_model(std::vector<float> observations, std::vector<float> pseudo_renges);
	std::vector<float> pseudo_range_estimator(float pseudo_pos);
};

#endif // !MALCOV_LOCALIZATION_C