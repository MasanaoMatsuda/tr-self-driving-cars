#include <iostream>
#include <vector>
#include <iterator>
#include "MarkovLocalizor.h"

using std::cout;
using std::endl;
using std::vector;
using std::size_t;
 

int main()
{
	int map_size = 25;
	float position_stdev = 1.0f;
	float control_stdev = 1.0f;
	float observation_stdev = 1.0f;
	float movement_per_timestep = 1.0f;
	vector<float> landmarks{ 5, 10, 12, 20 };

	// sensor data
	vector<vector<float> > sensor_observations{
		{1,7,12,21}, {0,6,11,20}, {5,10,19}, {4,9,18}, {3,8,17}, {2,7,16}, {1,6,15},
		{0,5,14}, {4,13}, {3,12}, {2,11}, {1,10}, {0,9}, {8}, {7}, {6}, {5}, {4}, 
		{3}, {2}, {1}, {0}, {}, {}, {} 
	};

	auto markov = MarkovLocalizor(map_size, landmarks, position_stdev, control_stdev, observation_stdev);
	
	vector<float> observations;
	for (size_t ts = 0; ts < sensor_observations.size(); ++ts)
	{
		if (sensor_observations[ts].empty())
			observations = { float(map_size) };
		else
			observations = sensor_observations[ts];
		
		markov.estimate_posteriors(movement_per_timestep, observations);
		markov.print_posterior();
	}

	return 0;
}