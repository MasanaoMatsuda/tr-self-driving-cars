#include "MarkovLocalizor.h"

using std::vector;
using std::size_t;

MarkovLocalizor::MarkovLocalizor(const int map_size, const std::vector<float> landmarks, const float pos_stdev, const float control_stdev, const float observation_stdev)
	: map_size(map_size), landmarks(landmarks), pos_stdev(pos_stdev), control_stdev(control_stdev), observation_stdev(observation_stdev)
{
	priors = initialize_priors();
	posteriors = initialize_posteriors();
};


void MarkovLocalizor::estimate_posteriors(float movement_per_timestep, vector<float> observations)
{
	for (float pseudo_pos = 0; pseudo_pos < map_size; ++pseudo_pos)
	{
		float motion_prob = motion_model(pseudo_pos, movement_per_timestep);
		vector<float> pseudo_ranges = pseudo_range_estimator(pseudo_pos);
		float observation_prob = observation_model(observations, pseudo_ranges);
		posteriors[pseudo_pos] = motion_prob * observation_prob;
	}

	posteriors = statistics.norm_vector(posteriors);
	priors = posteriors;
}


void MarkovLocalizor::print_posterior()
{
	for (size_t i = 0; i < map_size; ++i)
		std::cout << posteriors[i] << " ";
	std::cout << "\n" << std::endl;
}


vector<float> MarkovLocalizor::initialize_priors()
{
	vector<float> priors(map_size, 0.0f);

	float prob = 1 / ((2 * pos_stdev + 1) * landmarks.size());

	for (auto mark = std::begin(landmarks), last = std::end(landmarks); mark != last; ++mark)
	{
		priors.at(*mark) += prob;
		for (int stdev = 1; stdev <= pos_stdev; ++stdev)
		{
			priors.at(int(*mark - stdev + map_size) % map_size) += prob;
			priors.at(int(*mark + stdev + map_size) % map_size) += prob;
		}
	}
	return priors;
}


vector<float> MarkovLocalizor::initialize_posteriors()
{
	vector<float> posteriors(map_size, 0.0f);
	return posteriors;
}

float MarkovLocalizor::motion_model(const float pseudo_position, const float movement)
{
	float position_prob = 0.0f;
	for (float i = 0; i < map_size; ++i)
	{
		float distance = pseudo_position - i;
		float transition_prob = statistics.norm_pdf(distance, movement, control_stdev);
		position_prob += transition_prob * priors[i];
	}
	return position_prob;
}


vector<float> MarkovLocalizor::pseudo_range_estimator(const float pseudo_pos)
{
	vector<float> pseudo_ranges;
	for (int i = 0, num_of_landmarks = landmarks.size(); i < num_of_landmarks; ++i)
	{
		float difference = landmarks[i] - pseudo_pos;
		if (difference >= 0.0) pseudo_ranges.push_back(difference);
	}

	sort(pseudo_ranges.begin(), pseudo_ranges.end());

	return pseudo_ranges;
}


float MarkovLocalizor::observation_model(std::vector<float> observations, std::vector<float> pseudo_ranges)
{
	float distance_prob = 1.0f;
	for (size_t i = 0; i < observations.size(); ++i)
	{
		float pseudo_range_min;
		if (pseudo_ranges.size() > 0)
		{	// if distance vector exists
			pseudo_range_min = pseudo_ranges[0];
			pseudo_ranges.erase(pseudo_ranges.begin());
		}
		else
		{	// no or negative distances -> set to a large number
			pseudo_range_min = std::numeric_limits<const float>::infinity();
		}
		
		distance_prob *= statistics.norm_pdf(observations[i], pseudo_range_min, observation_stdev);
	}
	return distance_prob;
}