#include "Statistics.h"

float Statistics::norm_pdf(float x, float mu, float std)
{
	float constant = 1 / sqrt(2 * M_PI);
	return (constant / std) * exp(-0.5 * pow((x - mu) / std, 2));
}


std::vector<float> Statistics::norm_vector(std::vector<float> input_vec)
{
	size_t length = input_vec.size();

	std::vector<float> output_vec;
	output_vec.resize(length);

	float sum = 0.0f;
	for (int i = 0; i < length; ++i)
		sum += input_vec[i];

	for (int i = 0; i < length; ++i)
		output_vec[i] = input_vec[i] / sum;

	return output_vec;
}