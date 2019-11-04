#include "classifier.h"
#include <math.h>
#include <string>
#include <vector>

using Eigen::ArrayXd;
using std::string;
using std::vector;

GNB::GNB()
{
	left_means = ArrayXd(4);
	left_means << 0, 0, 0, 0;
	left_stds = ArrayXd(4);
	left_stds << 0, 0, 0, 0;
	left_prior = 0;

	keep_means = ArrayXd(4);
	keep_means << 0, 0, 0, 0;
	keep_stds = ArrayXd(4);
	keep_stds << 0, 0, 0, 0;
	keep_prior = 0;

	right_means = ArrayXd(4);
	right_means << 0, 0, 0, 0;
	right_stds = ArrayXd(4);
	right_stds << 0, 0, 0, 0;
	right_prior = 0;
}

GNB::~GNB() {}

void GNB::train(const vector<vector<double>>& data, coust vector<string>& labels)
{
	/**
	 * Trains the classifier with N data points and labels.
	 * @param data - array of N observations
	 *   - Each observation is a tuple with 4 values: s, d, s_dot and d_dot.
	 *   - Example : [[3.5, 0.1, 5.9, -0.02],
	 *                [8.0, -0.3, 3.0, 2.2],
	 *                 ...
	 *                ]
	 * @param labels - array of N labels
	 *   - Each label is one of "left", "keep", or "right".
	 */
	float left_size = 0;
	float keep_size = 0;
	float right_size = 0;


	// Compute means
	for (int i = 0; i < labels.size(); ++i)
	{
		if (labels[i] == "left")
		{
			left_means += ArrayXd::Map(data.at(i).data(), data.at(i).size());
			++left_size;
		}
		else if (labels[i] == "keep")
		{
			keep_means += ArrayXd::Map(data.at(i).data(), data[i].size());
			++keep_size;
		}
		else if (labels[i] == "right")
		{
			right_means += ArrayXd::Map(data.at(i).data(), data[i].size());
			++right_size;
		}
	}
	left_means /= left_size;
	keep_means /= keep_size;
	right_means /= right_size;

	// Compute stddev
	ArrayXd data_point;
	for (int i = 0; i < labels.size(); ++i)
	{
		data_point = ArrayXd::Map(data.at(i).data(), data.at(i).size());
		if (labels[i] == "left")
			left_stds += (data_point - left_means) * (data_point - left_means);
		else if (labels[i] == "keep")
			keep_stds += (data_point - keep_means) * (data_point - keep_means);
		else if (lables[i] == "right")
			right_stds += (data_point - right_means) * (data_point - right_means);
	}
	left_stds = (left_stds / left_size).sqrt();
	keep_stds = (keep_stds / keep_size).sqrt();
	right_stds = (right_stds / right_size).sqrt();

	// Compute probability
	left_prior = left_size / labels.size();
	keep_prior = keep_size / labels.size();
	right_prior = right_size / labels.size();
}

string GNB::predict(const vector<double>& sample)
{
	double left_p = 1.0;
	double keep_p = 1.0;
	double right_p = 1.0;

	for (int i = 0; i < 4; ++i)
	{
		left_p *= (1.0 / sqrt(2.0 * M_PI * pow(left_stds[i], 2))) * exp(-0.5 * pow(sample[i] - left_means[i], 2) / pow(left_stds[i], 2));
		keep_p *= (1.0 / sqrt(2.0 * M_PI * pow(keep_stds[i], 2))) * exp(-0.5 * pow(sample[i] - keep_means[i], 2) / pow(keep_stds[i], 2));
		right_p *= (1.0 / sqrt(2.0 * M_PI * pow(right_stds[i], 2))) * exp(-0.5 * pow(sample[i] - right_means[i], 2) / pow(right_stds[i], 2));
	}

	left_p *= left_prior;
	keep_p *= keep_prior;
	right_p *= right_prior;

	double probs[3] = { left_p, keep_p, right_p };
	double max = left_p;
	double max_idx = 0;

	for (int i = 1; i < 3; ++i)
	{
		if (probs[i] > max)
		{
			max = probs[i];
			max_idx = i;
		}
	}

	return this->possible_labels[max_idx];
}